#!/bin/bash

# Configurație
PROJECT_ROOT=$(pwd)
BUILD_DIR="$PROJECT_ROOT/build/ios"
TOOLCHAIN_FILE="$PROJECT_ROOT/ios.toolchain.cmake"
PLATFORM=OS
ARCHS="arm64"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1

# Rularea cmake
cmake "$PROJECT_ROOT" \
  -G "Unix Makefiles" \

  -DCMAKE_OSX_SYSROOT=iphoneos \
  -DCMAKE_OSX_ARCHITECTURES="$ARCHS" \
  -DPLATFORM="$PLATFORM" \
  -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . --config Release
