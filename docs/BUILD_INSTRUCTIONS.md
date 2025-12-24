# Build Instructions

## ⚠️ IMPORTANT: See [INSTALL_DEPENDENCIES.md](INSTALL_DEPENDENCIES.md) for up-to-date dependency installation!

## Prerequisites

### Required
- **CMake** 3.20+
- **C++20 Compiler** (GCC 10+, Clang 12+, MSVC 2019+)
- **Vulkan SDK** 1.3+ (or at least headers)
- **GLFW3** (for windowing)

### Quick Dependency Check

```bash
./scripts/check_dependencies.sh
```

### Install on Ubuntu/Debian

```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    libvulkan-dev \
    libglfw3-dev \
    glslang-tools
```

**If packages aren't available**, see [INSTALL_DEPENDENCIES.md](INSTALL_DEPENDENCIES.md) for alternatives.

## Build Steps

### 1. Clone/Setup

```bash
cd /workspace
```

### 2. Compile Shaders

```bash
# Make script executable
chmod +x scripts/compile_shaders.sh

# Run compilation
./scripts/compile_shaders.sh

# Or manually:
cd Engine/shaders
glslc triangle.vert -o triangle.vert.spv
glslc triangle.frag -o triangle.frag.spv
glslc cull.comp -o cull.comp.spv
cd ../..
```

### 3. Configure CMake

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
```

### 4. Build

```bash
cmake --build . -j$(nproc)
```

### 5. Run

```bash
cd bin
./EnjinEditor
# or
./ExampleTriangle
```

## Build Options

```bash
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DENJIN_BUILD_EDITOR=ON \
    -DENJIN_BUILD_EXAMPLES=ON \
    -DENJIN_BUILD_TESTS=OFF
```

## Troubleshooting

### "Cannot find Vulkan"
```bash
export VULKAN_SDK=/path/to/vulkan/sdk
export PATH=$VULKAN_SDK/bin:$PATH
export LD_LIBRARY_PATH=$VULKAN_SDK/lib:$LD_LIBRARY_PATH
```

### "Cannot find GLFW"
```bash
sudo apt-get install libglfw3-dev
```

### "glslc not found"
```bash
sudo apt-get install glslang-tools
# Or install Vulkan SDK which includes glslc
```

### "C++ compiler not found"
```bash
sudo apt-get install build-essential g++
```

## Expected Output

After successful build:
- `build/bin/EnjinEditor` - Editor executable
- `build/bin/ExampleTriangle` - Triangle example
- `build/lib/libEnjinCore.a` - Core library
- `build/lib/libEnjinEngine.a` - Engine library

## Running

Make sure shader `.spv` files are accessible:
- They're copied to `build/bin/shaders/` automatically
- Or place them in the same directory as executable
- Or use absolute paths

## Next Steps

See [GETTING_STARTED.md](GETTING_STARTED.md) for running instructions.
