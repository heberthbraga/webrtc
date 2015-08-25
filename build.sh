WORKING_DIR=`pwd`

WEBRTC_LIB_DIR=$WORKING_DIR/lib
WEBRTC_SRC_DIR=$WORKING_DIR/src
WEBRTC_IOS_DIR=$WORKING_DIR/webrtc-ios
DEPOT_TOOLS=$WORKING_DIR/depot_tools

#base function for configuring the build with ninja using depot tools
function build_base() {
  export GYP_GENERATORS="ninja,xcode-ninja"
  export GYP_DEFINES="build_with_libjingle=1 build_with_chromium=0 libjingle_objc=1"

  if [[ :$PATH: == *:"$DEPOT_TOOLS":* ]] ; then
    echo $PATH
  else
    PATH="$PATH:$DEPOT_TOOLS"
  fi
}

# We are not building for ios simulator

#function build_iossimX86() {
#  echo "-- building WebRTC for the iOS simulator X86"
#  build_base
#  export GYP_DEFINES="$GYP_DEFINES OS=ios target_arch=ia32"
#  export GYP_GENERATOR_FLAGS="$GYP_GENERATOR_FLAGS output_dir=out_ios_x86"
#  export GYP_CROSSCOMPILE=1
#  pushd src
#  gclient runhooks
#  ninja -C $WEBRTC_SRC_DIR/out_ios_x86/Debug-iphonesimulator iossim AppRTCDemo
#  popd
#}

#function build_iossimX64() {
#  echo "-- building WebRTC for the iOS simulator X64"
#  build_base
#  export GYP_DEFINES="$GYP_DEFINES OS=ios target_arch=x64 target_subarch=arm64"
#  export GYP_GENERATOR_FLAGS="$GYP_GENERATOR_FLAGS output_dir=out_ios_x64"
#  export GYP_CROSSCOMPILE=1
#  pushd $WEBRTC_SRC_DIR
#  gclient runhooks
#  ninja -C $WEBRTC_SRC_DIR/out_ios_x64/Debug-iphonesimulator iossim AppRTCDemo
#  popd
#}

##############################################

#build webrtc for 32bits devices
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

#build webrtc for 64bits devices
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

#build webrtc for osx 64bits
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

function build_all() {
  build_iosdeviceX86
  build_iosdeviceX64
}


function combine_libs_x86() {
  echo "-- combining libraries for armv7 architecture"
  libtool -static -o $WEBRTC_SRC_DIR/out_ios_x86/Debug-iphoneos/libWebRTC-ios.a $WEBRTC_SRC_DIR/out_ios_x86/Debug-iphoneos/*.a
  strip -S -x -o $WEBRTC_SRC_DIR/out_ios_x86/Debug-iphoneos/libWebRTC-ios-min.a -r $WEBRTC_SRC_DIR/out_ios_x86/Debug-iphoneos/libWebRTC-ios.a
  lipo -create $WEBRTC_SRC_DIR/out_ios_x86/Debug-iphoneos/libWebRTC-ios-min.a -output $WEBRTC_LIB_DIR/libWebRTC-armv7.a
}

function combine_libs_x64() {
  echo "-- combining libraries for arm64 architecture"
  libtool -static -o $WEBRTC_SRC_DIR/out_ios_x64/Debug-iphoneos/libWebRTC-ios.a $WEBRTC_SRC_DIR/out_ios_x64/Debug-iphoneos/*.a
  strip -S -x -o $WEBRTC_SRC_DIR/out_ios_x64/Debug-iphoneos/libWebRTC-ios-min.a -r $WEBRTC_SRC_DIR/out_ios_x64/Debug-iphoneos/libWebRTC-ios.a
  lipo -create $WEBRTC_SRC_DIR/out_ios_x64/Debug-iphoneos/libWebRTC-ios-min.a -output $WEBRTC_LIB_DIR/libWebRTC-arm64.a
}

function combine_libs_mac() {
  echo "-- combining libraries for osx x86_64"
  libtool -static -o $WEBRTC_SRC_DIR/out_mac/Debug/libWebRTC-mac.a $WEBRTC_SRC_DIR/out_mac/Debug/*.a
  strip -S -x -o $WEBRTC_SRC_DIR/out_mac/Debug/libWebRTC-mac-min.a -r $WEBRTC_SRC_DIR/out_mac/Debug/libWebRTC-mac.a
  lipo -create $WEBRTC_SRC_DIR/out_mac/Debug/libWebRTC-mac-min.a -output $WEBRTC_LIB_DIR/libWebRTC-x86-x64.a
}

function combine_all_libs() {
  #remove unused libraries
  echo "-- removing unesed libraries"
  rm -rv $WEBRTC_SRC_DIR/out_ios_x86/Debug-iphoneos/libapprtc_signaling.a $WEBRTC_SRC_DIR/out_ios_x86/Debug-iphoneos/libsocketrocket.a $WEBRTC_SRC_DIR/out_ios_x86/Debug-iphoneos/libWebRTC-ios-min.a $WEBRTC_SRC_DIR/out_ios_x86/Debug-iphoneos/libWebRTC-ios.a
  rm -rv $WEBRTC_SRC_DIR/out_ios_x64/Debug-iphoneos/libapprtc_signaling.a $WEBRTC_SRC_DIR/out_ios_x64/Debug-iphoneos/libsocketrocket.a $WEBRTC_SRC_DIR/out_ios_x64/Debug-iphoneos/libWebRTC-ios-min.a $WEBRTC_SRC_DIR/out_ios_x64/Debug-iphoneos/libWebRTC-ios.a

  combine_libs_x86
  combine_libs_x64
}

function generate_fat_file() {
  echo "-- Generating global libWebRTC.a static library"
  lipo -create $WEBRTC_LIB_DIR/*.a -output $WEBRTC_LIB_DIR/libWebRTC.a

  echo "-- Copying libWebRTC.a to webrtc ios folder"
  cp $WEBRTC_LIB_DIR/libWebRTC.a $WEBRTC_IOS_DIR/lib

  echo "-- Removing static libs"
  rm -rv $WEBRTC_LIB_DIR/*
}

#main function
function run() {
  if [ -z "$1" ]
  then
    echo "You have to provide the version of the release"
    echo "Usage: ./build.sh run <version>"
  else
    build_all

    combine_all_libs

    generate_fat_file

    echo "-- packing the folder files to a zip file"
    pushd $WEBRTC_IOS_DIR
    zip -r -q webrtc-ios-r$1.zip include/* lib/*
    popd

    echo "-- uploading the zip file to amazon s3"
    aws s3 cp $WEBRTC_IOS_DIR/webrtc-ios-r$1.zip s3://webrtc-smtrtc --acl public-read
  fi
}

# Run the function specified by the first parameter on the command line
$@