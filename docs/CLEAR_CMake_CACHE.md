# Clearing CMake Cache

## Problem
CMake is using an old cached version of CMakeLists.txt. You need to clear the cache.

## Solution: Delete Build Directory and Reconfigure

```powershell
# From D:\GitHub\enjin\build
cd D:\GitHub\enjin

# Delete build directory
Remove-Item -Recurse -Force build

# Create fresh build directory
mkdir build
cd build

# Configure fresh
cmake .. -G "Visual Studio 17 2022" -A x64
```

## Or Just Delete Cache Files

```powershell
# From build directory
cd D:\GitHub\enjin\build

# Delete cache
Remove-Item CMakeCache.txt -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force CMakeFiles -ErrorAction SilentlyContinue

# Reconfigure
cmake .. -G "Visual Studio 17 2022" -A x64
```

## Quick One-Liner

```powershell
cd D:\GitHub\enjin; Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue; mkdir build; cd build; cmake .. -G "Visual Studio 17 2022" -A x64
```

This will:
1. Delete the old build directory
2. Create a new one
3. Configure CMake fresh

The updated CMakeLists.txt will then be used, and GLFW will be downloaded automatically if not found.
