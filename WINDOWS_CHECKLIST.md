# Windows Build Checklist ✅

## Quick Start Guide for Windows 11

### ✅ Step 1: Install Prerequisites (15-30 minutes)

- [ ] **Visual Studio 2019 or 2022**
  - Download: https://visualstudio.microsoft.com/
  - Install "Desktop development with C++" workload
  - ✅ Verify: Open "x64 Native Tools Command Prompt"

- [ ] **CMake** (3.20+)
  - Download: https://cmake.org/download/
  - ✅ Verify: `cmake --version` in command prompt

- [ ] **Vulkan SDK**
  - Download: https://vulkan.lunarg.com/sdk/home
  - Install to default location
  - ✅ Verify: `vulkaninfo --summary` works

- [ ] **GLFW3** (choose one):
  - [ ] **Option A**: vcpkg (recommended)
    ```cmd
    git clone https://github.com/Microsoft/vcpkg.git
    cd vcpkg
    .\bootstrap-vcpkg.bat
    .\vcpkg install glfw3:x64-windows
    ```
  - [ ] **Option B**: Build from source
  - [ ] **Option C**: Pre-built binaries

### ✅ Step 2: Build (5 minutes)

```cmd
# Open "x64 Native Tools Command Prompt for VS 2019"

cd C:\path\to\EnjinEngine
mkdir build
cd build

# Configure (choose one):
cmake .. -G "Visual Studio 16 2019" -A x64
# OR for VS 2022:
cmake .. -G "Visual Studio 17 2022" -A x64

# If using vcpkg:
cmake .. -G "Visual Studio 16 2019" -A x64 -DCMAKE_TOOLCHAIN_FILE=C:\path\to\vcpkg\scripts\buildsystems\vcpkg.cmake

# Build
cmake --build . --config Release
```

### ✅ Step 3: Run (1 minute)

```cmd
cd bin\Release
ExampleTriangle.exe
```

**Expected**: Window opens with red triangle!

## 🎯 What You Should See

### Successful Build:
```
-- Configuring done
-- Generating done
-- Build files have been written to: ...
```

### Successful Run:
- Window opens
- Red triangle renders
- Console shows logs

## 🐛 Common Issues

### "Cannot find Vulkan"
```cmd
set VULKAN_SDK=C:\VulkanSDK\1.3.xxx.x
```

### "Cannot find GLFW"
- Use vcpkg toolchain file (see Step 2)
- Or set `-DGLFW3_DIR=...`

### Build Errors
- Check Visual Studio version (2019+)
- Ensure C++20 support enabled
- Verify all prerequisites installed

## 📚 Full Documentation

- **[WINDOWS_NEXT_STEPS.md](docs/WINDOWS_NEXT_STEPS.md)** - Detailed step-by-step guide
- **[WINDOWS_BUILD.md](docs/WINDOWS_BUILD.md)** - Complete Windows build documentation
- **[CROSS_COMPILATION.md](docs/CROSS_COMPILATION.md)** - Cross-compilation options

## ✅ Verification

After building, check:
- [ ] `build\bin\Release\ExampleTriangle.exe` exists
- [ ] No linker errors
- [ ] Executable runs
- [ ] Window opens
- [ ] Triangle renders

---

**Ready? Start with Step 1!**
