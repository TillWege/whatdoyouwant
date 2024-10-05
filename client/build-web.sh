#!/bin/bash
# build_web.sh - Script to build the project for the web using Emscripten

# Ensure the Emscripten environment is sourced
if [ -z "$EMSDK" ]; then
  echo "Emscripten environment not detected. Please run 'source /path/to/emsdk_env.sh' before running this script."
  exit 1
fi

# Create build directory if it doesn't exist
BUILD_DIR="build_web"
mkdir -p "$BUILD_DIR"

# Navigate to build directory
cd "$BUILD_DIR"

# Run CMake to configure and generate build system for Emscripten, disabling Wayland and enabling X11
emcmake cmake -DPLATFORM=Web ../

make

# Navigate back to the project root directory
cd ..

echo "Web build complete! The output files are located in the build_web directory."
