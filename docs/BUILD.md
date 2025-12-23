# Building Enjin Engine

## Prerequisites

### Required
- **CMake** 3.20 or higher
- **C++20 compatible compiler**:
  - GCC 10+ (Linux)
  - Clang 12+ (Linux/macOS)
  - MSVC 2019+ (Windows)
- **Vulkan SDK** 1.3+
  - Download from: https://vulkan.lunarg.com/
  - Ensure `VULKAN_SDK` environment variable is set

### Optional (for full features)
- **GLFW3** (for windowing)
  - Can be built from source or installed via package manager
- **shaderc** (for shader compilation)
- **Dear ImGui** (for editor - MIT licensed)

## Linux Build Instructions

### Install Dependencies

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential cmake libvulkan-dev vulkan-tools libglfw3-dev
```

#### Fedora/RHEL
```bash
sudo dnf install gcc-c++ cmake vulkan-devel glfw-devel
```

#### Arch Linux
```bash
sudo pacman -S base-devel cmake vulkan-devel glfw
```

### Build
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

## Windows Build Instructions

### Install Dependencies

1. Install **Visual Studio 2019 or later** with C++ workload
2. Install **CMake** from https://cmake.org/
3. Install **Vulkan SDK** from https://vulkan.lunarg.com/
4. Install **GLFW3**:
   - Download from https://www.glfw.org/
   - Or use vcpkg: `vcpkg install glfw3`

### Build with Visual Studio
```cmd
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
```

### Build with MinGW
```cmd
mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

## macOS Build Instructions

### Install Dependencies
```bash
brew install cmake vulkan-headers vulkan-loader glfw
```

Note: macOS doesn't have native Vulkan support. You'll need MoltenVK:
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

## Build Options

Configure with CMake options:

```bash
cmake .. \
  -DENJIN_BUILD_EDITOR=ON \
  -DENJIN_BUILD_TESTS=OFF \
  -DENJIN_BUILD_EXAMPLES=ON \
  -DCMAKE_BUILD_TYPE=Release
```

### Available Options
- `ENJIN_BUILD_EDITOR` - Build the editor (default: ON)
- `ENJIN_BUILD_TESTS` - Build unit tests (default: OFF)
- `ENJIN_BUILD_EXAMPLES` - Build example projects (default: OFF)
- `CMAKE_BUILD_TYPE` - Debug, Release, RelWithDebInfo, MinSizeRel

## Running

After building, executables will be in `build/bin/`:

```bash
# Run editor
./build/bin/EnjinEditor

# Run example
./build/bin/ExampleTriangle
```

## Troubleshooting

### Vulkan Not Found
- Ensure Vulkan SDK is installed
- Set `VULKAN_SDK` environment variable
- On Linux, install `libvulkan-dev` package

### GLFW Not Found
- Install GLFW3 development package
- Or build from source: https://www.glfw.org/

### Compilation Errors
- Ensure C++20 support is enabled
- Check compiler version meets requirements
- Verify all dependencies are installed

### Link Errors
- Ensure Vulkan libraries are in library path
- On Linux: `export LD_LIBRARY_PATH=$VULKAN_SDK/lib:$LD_LIBRARY_PATH`

## Development Setup

### Recommended IDE
- **Visual Studio Code** with C++ extension
- **CLion** (JetBrains)
- **Visual Studio** (Windows)

### Recommended Tools
- **Vulkan SDK** - Includes validation layers and tools
- **RenderDoc** - Graphics debugging
- **Nsight Graphics** - NVIDIA GPU debugging (Linux/Windows)
