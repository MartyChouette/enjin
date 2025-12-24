# Cross-Compilation Guide: Building for Windows from Linux

## ⚠️ Important: You Cannot Run Linux Binaries on Windows

A binary built on Linux will **NOT** run on Windows 11. They are different operating systems with different:
- Binary formats (ELF vs PE)
- System libraries
- ABIs (Application Binary Interfaces)

## Options for Building Windows Binaries

### Option 1: Build on Windows Directly (RECOMMENDED)

**Best approach**: Build directly on Windows 11.

#### Using Windows 11 Machine
```cmd
# Install Visual Studio 2019+ with C++ workload
# Install CMake from https://cmake.org/
# Install Vulkan SDK from https://vulkan.lunarg.com/

mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
```

#### Using WSL2 on Windows 11
You can use WSL2 (Windows Subsystem for Linux) on your Windows machine:
```bash
# In WSL2
sudo apt-get update
sudo apt-get install build-essential cmake libvulkan-dev libglfw3-dev

# But you still need to build for Windows, not Linux!
# See Option 2 for cross-compilation
```

### Option 2: Cross-Compile from Linux to Windows

**Complex but possible** - Requires MinGW-w64 toolchain.

#### Setup MinGW-w64 on Linux

```bash
# Ubuntu/Debian
sudo apt-get install mingw-w64

# Verify
x86_64-w64-mingw32-g++ --version
```

#### Cross-Compile GLFW

```bash
# Download GLFW source
git clone https://github.com/glfw/glfw.git
cd glfw
mkdir build-mingw && cd build-mingw

# Configure for Windows
cmake .. \
    -DCMAKE_SYSTEM_NAME=Windows \
    -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
    -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \
    -DCMAKE_INSTALL_PREFIX=/usr/x86_64-w64-mingw32

make -j$(nproc)
sudo make install
```

#### Cross-Compile Enjin Engine

```bash
# Set up toolchain file
cat > mingw-toolchain.cmake << 'EOF'
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
EOF

# Configure
mkdir build-windows && cd build-windows
cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=../mingw-toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . -j$(nproc)
```

**Limitations**:
- Vulkan SDK for Windows must be downloaded separately
- Some Windows-specific features may not work
- More complex setup

### Option 3: Use CI/CD (GitHub Actions)

**Best for automated builds** - Build on Windows runners.

Create `.github/workflows/build-windows.yml`:
```yaml
name: Build Windows

on: [push, pull_request]

jobs:
  build:
    runs-on: windows-latest
    steps:
      - uses: actions/checkout@v3
      - name: Install Vulkan SDK
        run: |
          # Download and install Vulkan SDK
      - name: Configure
        run: cmake -B build -G "Visual Studio 16 2019" -A x64
      - name: Build
        run: cmake --build build --config Release
      - name: Upload Artifacts
        uses: actions/upload-artifact@v3
        with:
          name: windows-binaries
          path: build/bin/Release/
```

### Option 4: Use Docker with Windows Container

**Advanced** - Requires Windows containers (not available on Linux hosts).

## Recommended Workflow

### For Development:
1. **Develop on Linux server** - Write code, test logic
2. **Build on Windows** - Use Windows machine or VM for final builds
3. **Test on Windows** - Run and debug on target platform

### For Production:
1. **Use CI/CD** - Automate Windows builds
2. **Or build locally on Windows** - Direct control

## Quick Setup: Building on Windows 11

### Prerequisites
1. **Visual Studio 2019+** with:
   - Desktop development with C++
   - Windows 10/11 SDK

2. **CMake** (3.20+)
   - Download from https://cmake.org/
   - Or: `winget install Kitware.CMake`

3. **Vulkan SDK**
   - Download from https://vulkan.lunarg.com/
   - Install to default location (e.g., `C:\VulkanSDK\1.x.x.x`)
   - Add to PATH automatically

4. **GLFW3**
   - Option A: Use vcpkg: `vcpkg install glfw3:x64-windows`
   - Option B: Build from source
   - Option C: Download pre-built binaries

### Build Steps

```cmd
# Open "x64 Native Tools Command Prompt for VS 2019"

# Clone/navigate to project
cd C:\path\to\EnjinEngine

# Configure
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64

# Build
cmake --build . --config Release

# Run
cd bin\Release
ExampleTriangle.exe
```

## Troubleshooting Cross-Compilation

### "Cannot find Windows libraries"
- Install MinGW-w64: `sudo apt-get install mingw-w64`
- Set up toolchain file (see above)

### "Vulkan not found"
- Download Windows Vulkan SDK separately
- Point CMake to Windows Vulkan SDK location

### "GLFW not found"
- Cross-compile GLFW first (see above)
- Or use pre-built Windows GLFW binaries

## Best Practice

**For a game engine targeting Windows:**

1. **Develop on Linux** ✅ (easier, faster iteration)
2. **Build on Windows** ✅ (native, fewer issues)
3. **Test on Windows** ✅ (target platform)

Use your Linux server for:
- Code development
- Linux builds/testing
- CI/CD automation

Use Windows machine for:
- Windows builds
- Final testing
- Distribution

## Summary

| Method | Complexity | Reliability | Speed |
|--------|-----------|-------------|-------|
| Build on Windows | ⭐ Easy | ⭐⭐⭐ Excellent | ⭐⭐⭐ Fast |
| Cross-compile | ⭐⭐⭐ Hard | ⭐⭐ Good | ⭐⭐ Medium |
| CI/CD | ⭐⭐ Medium | ⭐⭐⭐ Excellent | ⭐ Slow |

**Recommendation**: Build directly on Windows 11 for best results.
