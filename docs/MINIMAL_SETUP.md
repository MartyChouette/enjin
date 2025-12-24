# Minimal Setup Guide

## If You Can't Install All Packages

This guide helps you get the engine running with minimal dependencies.

## Absolute Minimum

### What You MUST Have:

1. **C++ Compiler**
   ```bash
   sudo apt-get install build-essential
   # Or on Fedora: sudo dnf install gcc-c++
   ```

2. **CMake**
   ```bash
   sudo apt-get install cmake
   # Or download from: https://cmake.org/download/
   ```

### What You Can Work Around:

#### Vulkan Headers (Required for compilation)

**Option 1: Package Manager**
```bash
sudo apt-get install libvulkan-dev
```

**Option 2: Vulkan SDK** (if package not available)
```bash
# Download from: https://vulkan.lunarg.com/sdk/home
# Extract anywhere
export VULKAN_SDK=/path/to/vulkan/sdk
export PATH=$VULKAN_SDK/bin:$PATH
```

**Option 3: Manual Headers** (advanced)
```bash
# Download Vulkan headers from GitHub
git clone https://github.com/KhronosGroup/Vulkan-Headers.git
# Point CMake to include directory
```

#### GLFW (Required for windowing)

**Option 1: Package Manager**
```bash
sudo apt-get install libglfw3-dev
```

**Option 2: Build from Source** (if package not available)
```bash
git clone https://github.com/glfw/glfw.git
cd glfw
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DGLFW_BUILD_DOCS=OFF -DGLFW_BUILD_TESTS=OFF
make -j$(nproc)
sudo make install
```

**Option 3: Use CMake FetchContent** (modify CMakeLists.txt)
```cmake
include(FetchContent)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git)
FetchContent_MakeAvailable(glfw)
```

#### Shader Compiler (Optional - can skip)

**If glslc not available:**

1. **Skip GPU Culling** - Engine uses CPU fallback
2. **Use Pre-compiled Shaders** - If someone provides .spv files
3. **Install Vulkan SDK** - Includes glslc

**To skip shader compilation:**
- Engine will work without compiled compute shader
- GPU culling will use CPU fallback
- Triangle shaders still needed (but can be provided pre-compiled)

## Step-by-Step Minimal Setup

### 1. Install Absolute Minimum

```bash
sudo apt-get update
sudo apt-get install build-essential cmake
```

### 2. Get Vulkan Headers

**Try package first:**
```bash
sudo apt-get install libvulkan-dev
```

**If that fails, download Vulkan SDK:**
```bash
# Download from: https://vulkan.lunarg.com/sdk/home
# Extract to ~/vulkan-sdk
export VULKAN_SDK=~/vulkan-sdk/1.x.x.x
export PATH=$VULKAN_SDK/bin:$PATH
```

### 3. Get GLFW

**Try package first:**
```bash
sudo apt-get install libglfw3-dev
```

**If that fails, build from source:**
```bash
git clone https://github.com/glfw/glfw.git
cd glfw && mkdir build && cd build
cmake .. && make -j$(nproc)
sudo make install
```

### 4. Build Engine

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### 5. Handle Shaders

**Option A: Compile (if glslc available)**
```bash
./scripts/compile_shaders.sh
```

**Option B: Skip GPU Culling**
- Engine will work with CPU fallback
- Triangle shaders still needed (can be provided)

**Option C: Use Pre-compiled**
- Get .spv files from someone else
- Place in `Engine/shaders/` directory

## What Works Without Full Setup

✅ **Core Engine** - Compiles and runs
✅ **Window Creation** - Works
✅ **Basic Rendering** - Works (if triangle shaders available)
✅ **ECS System** - Works
✅ **Day/Night Cycle** - Works
⚠️ **GPU Culling** - Uses CPU fallback
⚠️ **Bindless** - May not work without extensions

## Troubleshooting Specific Errors

### "Package 'X' not found"

**For each missing package:**
1. Try alternative package name
2. Try building from source
3. Check if feature can be disabled

### "Vulkan not found"

```bash
# Check if installed
dpkg -l | grep vulkan

# If not, download SDK manually
# Set VULKAN_SDK environment variable
```

### "GLFW not found"

```bash
# Try alternative name
sudo apt-get install libglfw-dev

# Or build from source (see above)
```

### "glslc not found"

**This is OK!** Engine will:
- Use CPU fallback for GPU culling
- Still work for basic rendering
- Just compile triangle shaders manually if needed

## Alternative: Use Docker

If package installation is problematic:

```dockerfile
FROM ubuntu:22.04
RUN apt-get update && apt-get install -y \
    build-essential cmake \
    libvulkan-dev libglfw3-dev
WORKDIR /workspace
```

## Check What You Have

Run the dependency checker:
```bash
./scripts/check_dependencies.sh
```

It will tell you exactly what's missing and what alternatives exist.

## Bottom Line

**Minimum to compile**: C++ compiler + CMake + Vulkan headers + GLFW

**Everything else is optional** - engine will work with reduced features!
