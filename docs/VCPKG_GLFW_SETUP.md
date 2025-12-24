# Setting Up GLFW with vcpkg on Windows

## Quick Fix: Install GLFW via vcpkg

If you're using vcpkg, make sure GLFW is installed:

```powershell
# Navigate to vcpkg directory
cd C:\vcpkg

# Install GLFW for x64 Windows
.\vcpkg install glfw3:x64-windows

# Verify installation
.\vcpkg list glfw3
```

You should see:
```
glfw3:x64-windows    3.3.8
```

## Then Configure CMake

```powershell
cd D:\GitHub\enjin\build
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
```

## Alternative: Let CMake Download GLFW Automatically

If you don't want to use vcpkg, just run CMake without the toolchain file:

```powershell
cd D:\GitHub\enjin\build
cmake .. -G "Visual Studio 17 2022" -A x64
```

CMake will automatically download and build GLFW using FetchContent.

## Troubleshooting

### "vcpkg install glfw3" fails
- Make sure vcpkg is bootstrapped: `.\bootstrap-vcpkg.bat`
- Check internet connection (vcpkg downloads from GitHub)
- Try: `.\vcpkg update` first

### CMake still can't find GLFW
- Verify vcpkg path: `echo $env:VCPKG_ROOT` (should show C:\vcpkg)
- Check installation: `.\vcpkg list glfw3`
- Try without vcpkg toolchain (auto-download)

### Build takes too long
- First build downloads and compiles GLFW (5-10 minutes)
- Subsequent builds are fast
- Or use pre-built vcpkg package

## Recommended Approach

**Option 1**: Install via vcpkg (faster, reusable)
```powershell
C:\vcpkg\vcpkg.exe install glfw3:x64-windows
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
```

**Option 2**: Auto-download (easier, but slower first time)
```powershell
cmake .. -G "Visual Studio 17 2022" -A x64
```

Both work! Choose based on your preference.
