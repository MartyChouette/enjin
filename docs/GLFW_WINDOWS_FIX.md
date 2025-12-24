# Fixing GLFW on Windows

## Error: `Could not find a package configuration file provided by "glfw3"`

This means CMake can't find GLFW. Here are solutions:

## Solution 1: Use vcpkg (RECOMMENDED)

### Install vcpkg:
```cmd
# Clone vcpkg
git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
cd C:\vcpkg

# Bootstrap
.\bootstrap-vcpkg.bat

# Install GLFW
.\vcpkg install glfw3:x64-windows
```

### Configure CMake with vcpkg:
```cmd
cd D:\GitHub\enjin\build
cmake .. -G "Visual Studio 17 2022" -A x64 ^
    -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
```

## Solution 2: Download Pre-built GLFW

1. Download GLFW from: https://www.glfw.org/download.html
2. Extract to `C:\glfw` (or any location)
3. Configure CMake with path:
```cmd
cmake .. -DGLFW3_DIR=C:\glfw\lib\cmake\glfw3
```

## Solution 3: Build GLFW from Source

```cmd
# Clone GLFW
git clone https://github.com/glfw/glfw.git
cd glfw
mkdir build
cd build

# Configure
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_INSTALL_PREFIX=C:\glfw

# Build and install
cmake --build . --config Release
cmake --install . --config Release

# Then configure your project:
cd D:\GitHub\enjin\build
cmake .. -DGLFW3_DIR=C:\glfw\lib\cmake\glfw3
```

## Solution 4: Use FetchContent (Automatic)

The updated CMakeLists.txt will automatically download GLFW if it's not found. Just run:

```cmd
cmake .. -G "Visual Studio 17 2022" -A x64
```

This will download and build GLFW automatically.

## Quick Test

After configuring, you should see:
```
-- Found GLFW: ...
-- GLFW found via EnjinCore
```

## Troubleshooting

### "vcpkg not found"
- Make sure vcpkg is installed
- Use full path: `-DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake`

### "GLFW still not found"
- Check vcpkg installation: `vcpkg list glfw3`
- Try Solution 4 (FetchContent) - it will download automatically

### "Download failed"
- Check internet connection
- Try manual download (Solution 2)

---

**Recommended**: Use Solution 1 (vcpkg) for best compatibility with Windows development.
