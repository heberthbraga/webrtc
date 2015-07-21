WEBRTC_LIB_DIR="/Users/heberthbraga/Workspace/quimbik/webrtc/lib"
WEBRTC_SRC_DIR="/Users/heberthbraga/Workspace/quimbik/webrtc/src"
DEPOT_TOOLS="/Users/heberthbraga/Workspace/quimbik/webrtc/depot_tools"

function build_base() {
  export GYP_GENERATORS="ninja,xcode-ninja"
  export GYP_DEFINES="build_with_libjingle=1 build_with_chromium=0 libjingle_objc=1"

  if [[ :$PATH: == *:"$DEPOT_TOOLS":* ]] ; then
    echo $PATH
  else
    PATH="$PATH:$DEPOT_TOOLS"
  fi
}

function build_iossimX86() {
  echo "-- building WebRTC for the iOS simulator X86"
  build_base
  export GYP_DEFINES="$GYP_DEFINES OS=ios target_arch=ia32"
  export GYP_GENERATOR_FLAGS="$GYP_GENERATOR_FLAGS output_dir=out_ios_x86"
  export GYP_CROSSCOMPILE=1
  pushd src
  gclient runhooks
  ninja -C $WEBRTC_SRC_DIR/out_ios_x86/Debug-iphonesimulator iossim AppRTCDemo
  popd
}
 
function build_iosdeviceX86() {
  echo "-- building WebRTC for iOS devices X86"
  build_base
  export GYP_DEFINES="$GYP_DEFINES OS=ios target_arch=arm"
  export GYP_GENERATOR_FLAGS="$GYP_GENERATOR_FLAGS output_dir=out_ios_x86"
  export GYP_CROSSCOMPILE=1
  pushd src
  gclient runhooks
  ninja -C $WEBRTC_SRC_DIR/out_ios_x86/Debug-iphoneos AppRTCDemo
  popd
}

function build_iossimX64() {
  echo "-- building WebRTC for the iOS simulator X64"
  build_base
  export GYP_DEFINES="$GYP_DEFINES OS=ios target_arch=x64 target_subarch=arm64"
  export GYP_GENERATOR_FLAGS="$GYP_GENERATOR_FLAGS output_dir=out_ios_x64"
  export GYP_CROSSCOMPILE=1
  pushd $WEBRTC_SRC_DIR
  gclient runhooks
  ninja -C $WEBRTC_SRC_DIR/out_ios_x64/Debug-iphonesimulator iossim AppRTCDemo
  popd
}
 
function build_iosdeviceX64() {
  echo "-- building WebRTC for iOS devices X64"
  build_base
  export GYP_DEFINES="$GYP_DEFINES OS=ios target_arch=arm64 target_subarch=arm64"
  export GYP_GENERATOR_FLAGS="$GYP_GENERATOR_FLAGS output_dir=out_ios_x64"
  export GYP_CROSSCOMPILE=1
  pushd $WEBRTC_SRC_DIR
  gclient runhooks
  ninja -C $WEBRTC_SRC_DIR/out_ios_x64/Debug-iphoneos AppRTCDemo
  popd
}

function build_osx64() {
  echo "-- building WebRTC for OSX x64"
  build_base
  export GYP_DEFINES="$GYP_DEFINES OS=mac target_arch=x64"
  export GYP_GENERATOR_FLAGS="$GYP_GENERATOR_FLAGS output_dir=out_mac"
  pushd $WEBRTC_SRC_DIR
  gclient runhooks
  ninja -C out_mac/Debug libjingle_peerconnection_objc_test
  popd
}
 
function combine_libs_x86() {
  echo "-- combining libraries"
  # libtool -static -o $WEBRTC_SRC_DIR/out_ios_x86/Debug-iphonesimulator/libWebRTC-sim.a $WEBRTC_SRC_DIR/out_ios_x86/Debug-iphonesimulator/*.a
  # strip -S -x -o $WEBRTC_SRC_DIR/out_ios_x86/Debug-iphonesimulator/libWebRTC-sim-min.a -r $WEBRTC_SRC_DIR/out_ios_x86/Debug-iphonesimulator/libWebRTC-sim.a
  libtool -static -o $WEBRTC_SRC_DIR/out_ios_x86/Debug-iphoneos/libWebRTC-ios.a $WEBRTC_SRC_DIR/out_ios_x86/Debug-iphoneos/*.a
  strip -S -x -o $WEBRTC_SRC_DIR/out_ios_x86/Debug-iphoneos/libWebRTC-ios-min.a -r $WEBRTC_SRC_DIR/out_ios_x86/Debug-iphoneos/libWebRTC-ios.a
  lipo -create $WEBRTC_SRC_DIR/out_ios_x86/Debug-iphoneos/libWebRTC-ios-min.a -output $WEBRTC_LIB_DIR/libWebRTC.a
  echo "The public headers are located in ./src/talk/app/webrtc/objc/public/*.h"
}

function combine_libs_x64() {
  echo "-- combining libraries"
  # libtool -static -o $WEBRTC_SRC_DIR/out_ios_x64/Debug-iphonesimulator/libWebRTC-sim.a $WEBRTC_SRC_DIR/out_ios_x64/Debug-iphonesimulator/*.a
  # strip -S -x -o $WEBRTC_SRC_DIR/out_ios_x64/Debug-iphonesimulator/libWebRTC-sim-min.a -r $WEBRTC_SRC_DIR/out_ios_x64/Debug-iphonesimulator/libWebRTC-sim.a
  libtool -static -o $WEBRTC_SRC_DIR/out_ios_x64/Debug-iphoneos/libWebRTC-ios.a $WEBRTC_SRC_DIR/out_ios_x64/Debug-iphoneos/*.a
  strip -S -x -o $WEBRTC_SRC_DIR/out_ios_x64/Debug-iphoneos/libWebRTC-ios-min.a -r $WEBRTC_SRC_DIR/out_ios_x64/Debug-iphoneos/libWebRTC-ios.a
  lipo -create $WEBRTC_SRC_DIR/out_ios_x64/Debug-iphoneos/libWebRTC-ios-min.a -output $WEBRTC_LIB_DIR/libWebRTC.a
  echo "The public headers are located in ./src/talk/app/webrtc/objc/public/*.h"
}
 
function run_simulatorX86() {
  echo "-- running webrtc appdemo on iOS simulator"
  $WEBRTC_SRC_DIR/out_ios_x86/Debug-iphonesimulator/iossim $WEBRTC_SRC_DIR/out_ios_x86/Debug-iphonesimulator/AppRTCDemo.app
}
 
function run_on_deviceX86() {
  echo "-- launching on device"
  ideviceinstaller -d -i $WEBRTC_SRC_DIR/out_ios_x86/Debug-iphoneos/AppRTCDemo.app
  echo "-- launch complete"
}

function run_on_deviceX64() {
  echo "-- launching on device"
  ideviceinstaller -d -i $WEBRTC_SRC_DIR/out_ios_x64/Debug-iphoneos/AppRTCDemo.app
  echo "-- launch complete"
}

function build_allX86() {
  build_iosdeviceX86 && combine_libs_x86
}

function build_allX64() {
  build_iosdeviceX64 && combine_libs_x64
}
 
# Run the function specified by the first parameter on the command line
$@