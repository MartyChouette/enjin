#!/bin/bash

# Shader Compilation Script
# Compiles all GLSL shaders to SPIR-V format

set -e

echo "Compiling Enjin Engine Shaders..."

# Check if glslc is available
if ! command -v glslc &> /dev/null; then
    echo "ERROR: glslc not found!"
    echo "Please install Vulkan SDK: https://vulkan.lunarg.com/"
    echo "Or install via package manager:"
    echo "  Ubuntu/Debian: sudo apt-get install glslang-tools"
    exit 1
fi

# Create output directory
mkdir -p Engine/shaders

# Compile shaders
echo "Compiling vertex shader..."
glslc Engine/shaders/triangle.vert -o Engine/shaders/triangle.vert.spv

echo "Compiling fragment shader..."
glslc Engine/shaders/triangle.frag -o Engine/shaders/triangle.frag.spv

echo "Compiling compute shader..."
glslc Engine/shaders/cull.comp -o Engine/shaders/cull.comp.spv

echo "Shader compilation complete!"
echo "Output: Engine/shaders/*.spv"
