# PowerShell CMake Commands

## Important: PowerShell vs CMD Syntax

**PowerShell uses backticks (`) for line continuation, NOT `^`**

## Correct PowerShell Commands

### Configure CMake (Single Line)
```powershell
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
```

### Configure CMake (Multi-line with backticks)
```powershell
cmake .. -G "Visual Studio 17 2022" -A x64 `
    -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
```

### Configure CMake (Without vcpkg - auto-downloads GLFW)
```powershell
cmake .. -G "Visual Studio 17 2022" -A x64
```

### Build
```powershell
cmake --build . --config Release
```

## Full Build Sequence (PowerShell)

```powershell
# Navigate to project
cd D:\GitHub\enjin

# Create build directory
mkdir build -ErrorAction SilentlyContinue
cd build

# Configure (choose one):

# Option 1: With vcpkg (single line)
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake

# Option 2: Without vcpkg (auto-downloads GLFW)
cmake .. -G "Visual Studio 17 2022" -A x64

# Build
cmake --build . --config Release
```

## Common Mistakes

### ❌ Wrong (CMD syntax in PowerShell):
```powershell
cmake .. -G "Visual Studio 17 2022" -A x64 ^
    -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
```

### ✅ Correct (PowerShell):
```powershell
cmake .. -G "Visual Studio 17 2022" -A x64 `
    -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
```

Or just use a single line!

## Quick Reference

| Task | PowerShell Command |
|------|-------------------|
| Configure | `cmake .. -G "Visual Studio 17 2022" -A x64` |
| Build | `cmake --build . --config Release` |
| Clean | `cmake --build . --target clean` |
| Configure with vcpkg | `cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake` |
