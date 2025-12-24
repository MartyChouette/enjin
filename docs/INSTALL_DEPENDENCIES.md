# Installing Dependencies - Updated Guide

## Platform-Specific Instructions

### Ubuntu 20.04 / Debian 11+

```bash
# Update package list
sudo apt-get update

# Install build essentials
sudo apt-get install -y build-essential cmake

# Install Vulkan SDK
# Option 1: From package manager (may be older version)
sudo apt-get install -y libvulkan-dev vulkan-tools vulkan-validationlayers

# Option 2: Download from LunarG (RECOMMENDED)
# Visit: https://vulkan.lunarg.com/sdk/home
# Download latest SDK for your platform
# Extract and set environment variables:
# export VULKAN_SDK=/path/to/vulkan/sdk
# export PATH=$VULKAN_SDK/bin:$PATH
# export LD_LIBRARY_PATH=$VULKAN_SDK/lib:$LD_LIBRARY_PATH

# Install GLFW
sudo apt-get install -y libglfw3-dev

# Install glslang (shader compiler)
sudo apt-get install -y glslang-tools
# OR if not available:
sudo apt-get install -y glslang-dev
```

### Ubuntu 22.04+

```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    libvulkan-dev \
    vulkan-tools \
    libglfw3-dev \
    glslang-tools
```

### Fedora / RHEL

```bash
sudo dnf install -y \
    gcc-c++ \
    cmake \
    vulkan-devel \
    vulkan-tools \
    glfw-devel \
    glslang
```

### Arch Linux

```bash
sudo pacman -S \
    base-devel \
    cmake \
    vulkan-devel \
    vulkan-tools \
    glfw \
    glslang
```

### macOS

```bash
# Install Homebrew if not installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake vulkan-headers vulkan-loader glfw glslang
```

## Alternative: Build from Source

If packages aren't available, you can build dependencies from source:

### GLFW from Source

```bash
git clone https://github.com/glfw/glfw.git
cd glfw
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
make -j$(nproc)
sudo make install
```

### Vulkan SDK from Source

```bash
# Download from: https://vulkan.lunarg.com/sdk/home
# Or use package manager version
```

## Verifying Installation

```bash
# Check Vulkan
vulkaninfo --summary || echo "Vulkan SDK not found"

# Check glslc
glslc --version || echo "glslc not found"

# Check GLFW
pkg-config --modversion glfw3 || echo "GLFW not found"

# Check CMake
cmake --version
```

## Troubleshooting

### "vulkan-tools not found"
- Try: `sudo apt-get install vulkan-validationlayers-dev`
- Or download Vulkan SDK manually from LunarG

### "glslang-tools not found"
- Try: `sudo apt-get install glslang-dev`
- Or use Vulkan SDK's glslc (included in SDK)
- Or build glslang from source

### "libglfw3-dev not found"
- Try: `sudo apt-get install libglfw-dev`
- Or build GLFW from source (see above)

### "CMake version too old"
- Download latest from: https://cmake.org/download/
- Or use snap: `sudo snap install cmake --classic`

## Minimal Setup (Just to Build)

If you can't install everything, minimum needed:

```bash
# Absolute minimum
sudo apt-get install build-essential cmake

# Then download pre-built binaries:
# - Vulkan SDK from LunarG
# - GLFW pre-built library
# - glslc from Vulkan SDK
```

## Docker Alternative

If package installation is problematic, use Docker:

```dockerfile
FROM ubuntu:22.04
RUN apt-get update && apt-get install -y \
    build-essential cmake \
    libvulkan-dev vulkan-tools \
    libglfw3-dev glslang-tools
```

## What's Actually Required

**Minimum to compile**:
- C++ compiler (GCC/Clang)
- CMake
- Vulkan headers (libvulkan-dev)
- GLFW headers (libglfw3-dev)

**To compile shaders**:
- glslc (from Vulkan SDK or glslang-tools)

**To run**:
- Vulkan loader (libvulkan1)
- GLFW library (libglfw3)

## Quick Check Script

```bash
#!/bin/bash
echo "Checking dependencies..."

check_cmd() {
    if command -v $1 &> /dev/null; then
        echo "✅ $1 found"
    else
        echo "❌ $1 not found"
    fi
}

check_cmd g++
check_cmd cmake
check_cmd glslc
check_cmd vulkaninfo

echo "Done!"
```

Save as `check_deps.sh` and run: `bash check_deps.sh`
