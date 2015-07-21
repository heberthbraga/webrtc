// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/atomicops.h"
#include "base/bind.h"
#include "base/command_line.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/stringprintf.h"
#include "base/timer/elapsed_timer.h"
#include "tools/gn/build_settings.h"
#include "tools/gn/commands.h"
#include "tools/gn/ninja_target_writer.h"
#include "tools/gn/ninja_writer.h"
#include "tools/gn/runtime_deps.h"
#include "tools/gn/scheduler.h"
#include "tools/gn/setup.h"
#include "tools/gn/standard_out.h"
#include "tools/gn/switches.h"
#include "tools/gn/target.h"

namespace commands {

namespace {

const char kSwitchCheck[] = "check";

// Called on worker thread to write the ninja file.
void BackgroundDoWrite(const Target* target) {
  NinjaTargetWriter::RunAndWriteFile(target);
  g_scheduler->DecrementWorkCount();
}

// Called on the main thread.
void ItemResolvedCallback(base::subtle::Atomic32* write_counter,
                          scoped_refptr<Builder> builder,
                          const BuilderRecord* record) {
  base::subtle::NoBarrier_AtomicIncrement(write_counter, 1);

  const Item* item = record->item();
  const Target* target = item->AsTarget();
  if (target) {
    g_scheduler->IncrementWorkCount();
    g_scheduler->ScheduleWork(base::Bind(&BackgroundDoWrite, target));
  }
}

// Returns a pointer to the target with the given file as an output, or null
// if no targets generate the file. This is brute force since this is an
// error condition and performance shouldn't matter.
const Target* FindTargetThatGeneratesFile(const Builder* builder,
                                          const SourceFile& file) {
  std::vector<const Target*> targets = builder->GetAllResolvedTargets();
  if (targets.empty())
    return nullptr;

  OutputFile output_file(targets[0]->settings()->build_settings(), file);
  for (const Target* target : targets) {
    for (const auto& cur_output : target->computed_outputs()) {
      if (cur_output == output_file)
        return target;
    }
  }
  return nullptr;
}

// Prints an error that the given file was present as a source or input in
// the given target(s) but was not generated by any of its dependencies.
void PrintInvalidGeneratedInput(const Builder* builder,
                                const SourceFile& file,
                                const std::vector<const Target*>& targets) {
  std::string err;

  const std::string target_str = targets.size() > 1 ? "targets" : "target";
  err += "The file:\n";
  err += "  " + file.value() + "\n";
  err += "is listed as an input or source for the " + target_str + ":\n";
  for (const Target* target : targets)
    err += "  " + target->label().GetUserVisibleName(false) + "\n";

  const Target* generator = FindTargetThatGeneratesFile(builder, file);
  if (generator) {
    err += "but this file was not generated by any dependencies of the " +
        target_str + ". The target\nthat generates the file is:\n  ";
    err += generator->label().GetUserVisibleName(false);
  } else {
    err += "but no targets in the build generate that file.";
  }

  Err(Location(), "Input to " + target_str + " not generated by a dependency.",
      err).PrintToStdout();
}

bool CheckForInvalidGeneratedInputs(Setup* setup) {
  std::multimap<SourceFile, const Target*> unknown_inputs =
      g_scheduler->GetUnknownGeneratedInputs();
  if (unknown_inputs.empty())
    return true;  // No bad files.

  int errors_found = 0;
  auto cur = unknown_inputs.begin();
  while (cur != unknown_inputs.end()) {
    errors_found++;
    auto end_of_range = unknown_inputs.upper_bound(cur->first);

    // Package the values more conveniently for printing.
    SourceFile bad_input = cur->first;
    std::vector<const Target*> targets;
    while (cur != end_of_range)
      targets.push_back((cur++)->second);

    PrintInvalidGeneratedInput(setup->builder(), bad_input, targets);
    OutputString("\n");
  }

  OutputString(
      "If you have generated inputs, there needs to be a dependency path "
      "between the\ntwo targets in addition to just listing the files. For "
      "indirect dependencies,\nthe intermediate ones must be public_deps. "
      "data_deps don't count since they're\nonly runtime dependencies. If "
      "you think a dependency chain exists, it might be\nbecause the chain "
      "is private. Try \"gn path\" to analyze.\n");

  if (errors_found > 1) {
    OutputString(base::StringPrintf("\n%d generated input errors found.\n",
                                    errors_found), DECORATION_YELLOW);
  }
  return false;
}

}  // namespace

const char kGen[] = "gen";
const char kGen_HelpShort[] =
    "gen: Generate ninja files.";
const char kGen_Help[] =
    "gn gen: Generate ninja files.\n"
    "\n"
    "  gn gen <out_dir>\n"
    "\n"
    "  Generates ninja files from the current tree and puts them in the given\n"
    "  output directory.\n"
    "\n"
    "  The output directory can be a source-repo-absolute path name such as:\n"
    "      //out/foo\n"
    "  Or it can be a directory relative to the current directory such as:\n"
    "      out/foo\n"
    "\n"
    "  See \"gn help\" for the common command-line switches.\n";

int RunGen(const std::vector<std::string>& args) {
  base::ElapsedTimer timer;

  if (args.size() != 1) {
    Err(Location(), "Need exactly one build directory to generate.",
        "I expected something more like \"gn gen out/foo\"\n"
        "You can also see \"gn help gen\".").PrintToStdout();
    return 1;
  }

  // Deliberately leaked to avoid expensive process teardown.
  Setup* setup = new Setup();
  if (!setup->DoSetup(args[0], true))
    return 1;

  if (base::CommandLine::ForCurrentProcess()->HasSwitch(kSwitchCheck))
    setup->set_check_public_headers(true);

  // Cause the load to also generate the ninja files for each target. We wrap
  // the writing to maintain a counter.
  base::subtle::Atomic32 write_counter = 0;
  setup->builder()->set_resolved_callback(
      base::Bind(&ItemResolvedCallback, &write_counter,
                 scoped_refptr<Builder>(setup->builder())));

  // Do the actual load. This will also write out the target ninja files.
  if (!setup->Run())
    return 1;

  Err err;
  // Write the root ninja files.
  if (!NinjaWriter::RunAndWriteFiles(&setup->build_settings(),
                                     setup->builder(),
                                     &err)) {
    err.PrintToStdout();
    return 1;
  }

  if (!WriteRuntimeDepsFilesIfNecessary(*setup->builder(), &err)) {
    err.PrintToStdout();
    return 1;
  }

  if (!CheckForInvalidGeneratedInputs(setup))
    return 1;

  base::TimeDelta elapsed_time = timer.Elapsed();

  if (!base::CommandLine::ForCurrentProcess()->HasSwitch(switches::kQuiet)) {
    OutputString("Done. ", DECORATION_GREEN);

    std::string stats = "Wrote " +
        base::IntToString(static_cast<int>(write_counter)) +
        " targets from " +
        base::IntToString(
            setup->scheduler().input_file_manager()->GetInputFileCount()) +
        " files in " +
        base::Int64ToString(elapsed_time.InMilliseconds()) + "ms\n";
    OutputString(stats);
  }

  return 0;
}

}  // namespace commands