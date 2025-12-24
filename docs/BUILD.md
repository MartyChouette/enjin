# Building Enjin Engine

## ⚠️ IMPORTANT: See [INSTALL_DEPENDENCIES.md](INSTALL_DEPENDENCIES.md) for up-to-date dependency installation!

## Prerequisites

### Required
- **CMake** 3.20 or higher
- **C++20 compatible compiler**:
  - GCC 10+ (Linux)
  - Clang 12+ (Linux/macOS)
  - MSVC 2019+ (Windows)
- **Vulkan SDK** 1.3+ (or at least headers)
- **GLFW3** (for windowing)

### Quick Dependency Check

```bash
./scripts/check_dependencies.sh
```

This will tell you exactly what's installed and what's missing.

### Installing Dependencies

**See [INSTALL_DEPENDENCIES.md](INSTALL_DEPENDENCIES.md) for detailed, up-to-date instructions.**

Quick check:
```bash
./scripts/check_dependencies.sh
```

## Linux Build Instructions

### Step 1: Install Dependencies

#### Ubuntu/Debian (Updated)
```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    libvulkan-dev \
    libglfw3-dev

# For shader compilation (choose one):
# Option A: Install glslang-tools
sudo apt-get install -y glslang-tools

# Option B: Use Vulkan SDK (RECOMMENDED)
# Download from: https://vulkan.lunarg.com/sdk/home
# Extract and set VULKAN_SDK environment variable
```

#### If Packages Not Available

**Vulkan SDK** (if libvulkan-dev not available):
```bash
# Download from LunarG: https://vulkan.lunarg.com/sdk/home
# Extract to /opt/vulkan-sdk
export VULKAN_SDK=/opt/vulkan-sdk/x.x.x.x
export PATH=$VULKAN_SDK/bin:$PATH
export LD_LIBRARY_PATH=$VULKAN_SDK/lib:$LD_LIBRARY_PATH
```

**GLFW** (if libglfw3-dev not available):
```bash
git clone https://github.com/glfw/glfw.git
cd glfw
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
sudo make install
```

**glslc** (if glslang-tools not available):
- Use the one included in Vulkan SDK
- Or build glslang from source: https://github.com/KhronosGroup/glslang

### Step 2: Compile Shaders

```bash
# Check if glslc is available
which glslc || echo "glslc not found - install Vulkan SDK or glslang-tools"

# Compile shaders
./scripts/compile_shaders.sh

# Or manually if script doesn't work:
cd Engine/shaders
glslc triangle.vert -o triangle.vert.spv
glslc triangle.frag -o triangle.frag.spv
glslc cull.comp -o cull.comp.spv
```

**Note**: If glslc is not available, you can:
1. Skip GPU culling for now (uses CPU fallback)
2. Use pre-compiled shaders (if provided)
3. Install Vulkan SDK which includes glslc

### Step 3: Build Engine

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

### Step 4: Run

```bash
cd bin
./ExampleTriangle
```

## Windows Build Instructions

### Install Dependencies

1. **Visual Studio 2019+** with C++ workload
2. **CMake** from https://cmake.org/
3. **Vulkan SDK** from https://vulkan.lunarg.com/
4. **GLFW3**:
   - Download from https://www.glfw.org/
   - Or use vcpkg: `vcpkg install glfw3`

### Build

```cmd
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
```

## macOS Build Instructions

### Install Dependencies

```bash
brew install cmake vulkan-headers vulkan-loader glfw glslang
```

**Note**: macOS doesn't have native Vulkan. You'll need MoltenVK:
```bash
brew install molten-vk
```

### Build

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(sysctl -n hw.ncpu)
```

## Troubleshooting

### "Cannot find Vulkan"
```bash
# Set Vulkan SDK path
export VULKAN_SDK=/path/to/vulkan/sdk
export PATH=$VULKAN_SDK/bin:$PATH
export LD_LIBRARY_PATH=$VULKAN_SDK/lib:$LD_LIBRARY_PATH

# Verify
vulkaninfo --summary
```

### "Cannot find GLFW"
```bash
# Install GLFW
sudo apt-get install libglfw3-dev

# Or build from source (see INSTALL_DEPENDENCIES.md)
```

### "glslc not found"
```bash
# Option 1: Install glslang-tools
sudo apt-get install glslang-tools

# Option 2: Use Vulkan SDK's glslc
export PATH=$VULKAN_SDK/bin:$PATH

# Option 3: Skip GPU culling (uses CPU fallback)
# Engine will work without it
```

### "C++ compiler not found"
```bash
sudo apt-get install build-essential g++
```

### Build Errors

**"Cannot find Vulkan"**:
- Install `libvulkan-dev` or download Vulkan SDK
- Set `VULKAN_SDK` environment variable

**"Cannot find GLFW"**:
- Install `libglfw3-dev` or build from source

**"CMake version too old"**:
- Download latest from https://cmake.org/download/
- Or use snap: `sudo snap install cmake --classic`

## Minimal Build (Without Optional Features)

If you can't install everything:

```bash
# Minimum required
sudo apt-get install build-essential cmake

# Then manually provide:
# - Vulkan headers (download SDK)
# - GLFW (build from source or download)
# - Skip shader compilation (use CPU fallback)
```

The engine will compile and run, but:
- GPU culling will use CPU fallback
- Some features may be limited

## Verification

After building, verify:

```bash
# Check executables
ls -lh build/bin/

# Check libraries
ls -lh build/lib/

# Check shaders
ls -lh build/bin/shaders/
```

## Next Steps

See [GETTING_STARTED.md](GETTING_STARTED.md) for running instructions.
