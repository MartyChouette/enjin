#!/bin/bash

# Dependency Check Script for Enjin Engine

echo "=== Enjin Engine Dependency Check ==="
echo ""

check_command() {
    if command -v $1 &> /dev/null; then
        echo "✅ $1: $(command -v $1)"
        if [ "$2" != "" ]; then
            $1 $2 2>&1 | head -1
        fi
        return 0
    else
        echo "❌ $1: NOT FOUND"
        return 1
    fi
}

check_package() {
    if dpkg -l | grep -q "^ii.*$1"; then
        echo "✅ Package $1: installed"
        return 0
    else
        echo "❌ Package $1: NOT INSTALLED"
        return 1
    fi
}

echo "=== Commands ==="
GCC_OK=$(check_command g++ --version)
CMAKE_OK=$(check_command cmake --version)
GLSLC_OK=$(check_command glslc --version)
VULKANINFO_OK=$(check_command vulkaninfo --summary 2>&1 | head -1)

echo ""
echo "=== Packages ==="
VULKAN_OK=$(check_package libvulkan-dev)
GLFW_OK=$(check_package libglfw3-dev)
GLSLANG_OK=$(check_package glslang-tools)

echo ""
echo "=== Summary ==="
if [ "$GCC_OK" = "0" ] && [ "$CMAKE_OK" = "0" ] && [ "$VULKAN_OK" = "0" ] && [ "$GLFW_OK" = "0" ]; then
    echo "✅ Core dependencies: OK"
    if [ "$GLSLC_OK" = "0" ]; then
        echo "✅ Shader compiler: OK"
    else
        echo "⚠️  Shader compiler: Missing (can use Vulkan SDK glslc)"
    fi
    echo ""
    echo "Ready to build!"
else
    echo "❌ Missing dependencies"
    echo ""
    echo "Install missing packages:"
    echo "  sudo apt-get install build-essential cmake libvulkan-dev libglfw3-dev"
    if [ "$GLSLC_OK" != "0" ]; then
        echo "  sudo apt-get install glslang-tools"
        echo "  OR install Vulkan SDK (includes glslc)"
    fi
fi
