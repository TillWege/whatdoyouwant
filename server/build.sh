#!/bin/bash
# build.sh - Script to build the project
BUILD_DIR="build"

cmake -B ./build

# Navigate to build directory
cd "$BUILD_DIR"

# Build the project using make
make

# Navigate back to the project root directory
cd ..

echo "Native build complete! The executable is located in the build_native directory."
