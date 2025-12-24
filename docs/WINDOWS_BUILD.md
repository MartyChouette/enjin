# Building for Windows 11

## Quick Start

### Prerequisites

1. **Visual Studio 2019 or 2022**
   - Install with "Desktop development with C++" workload
   - Includes Windows SDK and MSVC compiler

2. **CMake** (3.20+)
   - Download from https://cmake.org/download/
   - Or use: `winget install Kitware.CMake`
   - Add to PATH during installation

3. **Vulkan SDK**
   - Download from https://vulkan.lunarg.com/sdk/home
   - Install to default location (usually `C:\VulkanSDK\<version>`)
   - Installer automatically adds to PATH

4. **GLFW3**
   - **Option A (Recommended)**: Use vcpkg
     ```cmd
     git clone https://github.com/Microsoft/vcpkg.git
     cd vcpkg
     .\bootstrap-vcpkg.bat
     .\vcpkg install glfw3:x64-windows
     ```
   - **Option B**: Build from source
   - **Option C**: Download pre-built binaries from https://www.glfw.org/

### Build Steps

#### Using Visual Studio GUI

1. Open Visual Studio
2. File → Open → CMake...
3. Select `CMakeLists.txt` in project root
4. Build → Build All (or press F7)

#### Using Command Line

```cmd
# Open "x64 Native Tools Command Prompt for VS 2019" or "Developer Command Prompt"

# Navigate to project
cd C:\path\to\EnjinEngine

# Create build directory
mkdir build
cd build

# Configure (choose one):
# Visual Studio 2019:
cmake .. -G "Visual Studio 16 2019" -A x64

# Visual Studio 2022:
cmake .. -G "Visual Studio 17 2022" -A x64

# Or use default generator:
cmake ..

# Build
cmake --build . --config Release

# Or build Debug:
cmake --build . --config Debug
```

### Run

```cmd
cd build\bin\Release
ExampleTriangle.exe
```

## Using vcpkg (Recommended for GLFW)

### Setup vcpkg

```cmd
# Clone vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg

# Bootstrap
.\bootstrap-vcpkg.bat

# Install GLFW
.\vcpkg install glfw3:x64-windows

# Integrate with Visual Studio (optional)
.\vcpkg integrate install
```

### Configure CMake with vcpkg

```cmd
cmake .. ^
    -G "Visual Studio 16 2019" -A x64 ^
    -DCMAKE_TOOLCHAIN_FILE=C:\path\to\vcpkg\scripts\buildsystems\vcpkg.cmake
```

## Build Options

```cmd
cmake .. ^
    -G "Visual Studio 16 2019" -A x64 ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DENJIN_BUILD_EDITOR=ON ^
    -DENJIN_BUILD_EXAMPLES=ON ^
    -DENJIN_BUILD_TESTS=OFF
```

## Troubleshooting

### "Cannot find Vulkan"

```cmd
# Set environment variable
set VULKAN_SDK=C:\VulkanSDK\1.3.xxx.x

# Or add to System Environment Variables:
# Control Panel → System → Advanced → Environment Variables
# Add VULKAN_SDK = C:\VulkanSDK\1.3.xxx.x
```

### "Cannot find GLFW"

**If using vcpkg:**
```cmd
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\path\to\vcpkg\scripts\buildsystems\vcpkg.cmake
```

**If building GLFW manually:**
```cmd
# Download GLFW source
# Build with CMake
# Install to a known location
# Point CMake to it:
cmake .. -DGLFW3_DIR=C:\path\to\glfw\install\lib\cmake\glfw3
```

### "C++20 not supported"

- Ensure Visual Studio 2019 Update 16.11+ or Visual Studio 2022
- Check project properties → C/C++ → Language → C++ Language Standard → C++20

### "Linker errors"

- Ensure all dependencies are built for same architecture (x64)
- Check that Vulkan SDK is properly installed
- Verify GLFW is linked correctly

## Project Structure After Build

```
build/
├── bin/
│   ├── Release/
│   │   ├── EnjinEditor.exe
│   │   └── ExampleTriangle.exe
│   └── Debug/
│       └── ...
├── lib/
│   ├── Release/
│   │   ├── EnjinCore.lib
│   │   └── EnjinEngine.lib
│   └── Debug/
│       └── ...
└── ...
```

## Development Workflow

### Recommended Setup

1. **Develop on Linux server** (faster iteration, better tools)
2. **Build on Windows** (native target platform)
3. **Test on Windows** (actual runtime environment)

### Using Git

```bash
# On Linux server
git add .
git commit -m "Feature X"
git push

# On Windows machine
git pull
cmake --build build --config Release
```

### Using WSL2 on Windows

You can develop in WSL2 but still need to build for Windows:

```bash
# In WSL2
# Develop code here

# But build must target Windows:
# Either use cross-compilation (complex)
# Or build directly on Windows host
```

## Next Steps

- See [CROSS_COMPILATION.md](CROSS_COMPILATION.md) for cross-compiling from Linux
- See [BUILD.md](BUILD.md) for general build instructions
- See [GETTING_STARTED.md](GETTING_STARTED.md) for running the engine
