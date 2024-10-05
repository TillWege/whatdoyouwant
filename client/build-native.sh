#!/bin/bash
# build_native.sh - Script to build the project for native platforms

# Create build directory if it doesn't exist
BUILD_DIR="build_native"
mkdir -p "$BUILD_DIR"

# Run CMake to configure and generate build system for native
cmake -B ./build_native

# Navigate to build directory
cd "$BUILD_DIR"

# Build the project using make
make

# Navigate back to the project root directory
cd ..

echo "Native build complete! The executable is located in the build_native directory."
