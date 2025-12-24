# Windows Build Fix

## Error: `ninja: error: loading 'build.ninja': The system cannot find the file specified.`

This error means CMake hasn't been configured yet. You need to run `cmake` first to generate the build files.

## Correct Build Sequence

### Step 1: Configure CMake

```cmd
cd D:\GitHub\enjin
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
```

**Or if you prefer Ninja:**
```cmd
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release
```

**Or use default generator:**
```cmd
cmake ..
```

### Step 2: Build

**If using Visual Studio generator:**
```cmd
cmake --build . --config Release
```

**If using Ninja:**
```cmd
cmake --build . --config Release
# Or just:
ninja
```

## Quick Fix

If you're already in the `build` directory:

```cmd
# Go back to project root
cd ..

# Configure CMake
cmake -B build -G "Visual Studio 17 2022" -A x64

# Build
cmake --build build --config Release
```

## Common Issues

### "CMake not found"
- Install CMake from https://cmake.org/download/
- Or use Visual Studio's built-in CMake support

### "Generator not found"
- Try: `cmake .. -G "Visual Studio 16 2019" -A x64` (for VS 2019)
- Or: `cmake .. -G "Visual Studio 17 2022" -A x64` (for VS 2022)
- Or: `cmake ..` (uses default)

### "Vulkan not found"
- Install Vulkan SDK from https://vulkan.lunarg.com/
- Set `VULKAN_SDK` environment variable

### "GLFW not found"
- Install via vcpkg: `vcpkg install glfw3:x64-windows`
- Or download from https://www.glfw.org/

## Recommended: Use Visual Studio

**Easiest way:**
1. Open Visual Studio
2. File → Open → CMake...
3. Select `CMakeLists.txt` in project root
4. Build → Build All

This handles CMake configuration automatically!
