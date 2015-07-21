/*
core_seeds.h - C++ interface and ANSI C interface
Author: Luis Paris
Copyright (c) Centri Technology, Inc.
Copyright (c) The University of Mississippi.
*/

#ifndef INCLUDE_CORE_SEEDS_H
#define INCLUDE_CORE_SEEDS_H

#include <stdint.h>

//
// C++ Interface - use *ONLY* if compiling/linking in C++ mode
//

#ifdef __cplusplus

namespace Core {

enum {  //seed indexes
    NoSeedIdx = 0
  , HttpSeedIdx
  , HtmlSeedIdx
  , JsSeedIdx
  //add more...
};

extern const int NumSeeds();  //ret. number of seeds
extern const uint8_t *Seed( int index);  //ret. seed pointer given seed index (1-based); null if invalid index
extern const int SeedLen( int index);  //ret. seed length given seed index (1-based); -1 if invalid index

} //namespace Core

#endif //__cplusplus

/*
 * ANSI C Interface - use *ONLY* if compiling/linking in C mode
 */

#ifdef __cplusplus
extern "C" {
#endif

enum {
    NO_SEED_IDX = 0
  , HTTP_SEED_IDX
  , HTML_SEED_IDX
  , JS_SEED_IDX
  //add more...
};

extern const int getNumSeeds();  //ret. number of seeds
extern const uint8_t *getSeed( int index);  //ret. seed pointer given seed index (1-based); returns null if invalid index
extern const int getSeedLen( int index);  //ret. seed length given seed index (1-based); -1 if invalid index

#ifdef __cplusplus
}
#endif

#endif //CORE_SEEDS_H
