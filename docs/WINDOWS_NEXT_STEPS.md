# Next Steps: Building on Windows 11

## ✅ Current Status

The engine codebase is ready to build! It uses embedded shaders (no external files needed).

## 📋 Step-by-Step Checklist

### Step 1: Install Prerequisites

#### 1.1 Install Visual Studio 2019 or 2022
- Download from: https://visualstudio.microsoft.com/
- **Required workload**: "Desktop development with C++"
- This includes:
  - MSVC compiler
  - Windows SDK
  - CMake tools (optional but helpful)

#### 1.2 Install CMake
- Download from: https://cmake.org/download/
- Choose "Windows x64 Installer"
- **Important**: Check "Add CMake to system PATH" during installation
- Or use: `winget install Kitware.CMake`

#### 1.3 Install Vulkan SDK
- Download from: https://vulkan.lunarg.com/sdk/home
- Choose latest version for Windows
- Run installer (default location: `C:\VulkanSDK\<version>`)
- **Important**: Installer adds to PATH automatically
- Verify: Open new command prompt, run `vulkaninfo --summary`

#### 1.4 Install GLFW3

**Option A: Using vcpkg (Recommended)**
```cmd
# Install vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Install GLFW
.\vcpkg install glfw3:x64-windows

# Note the installation path (usually C:\vcpkg\installed\x64-windows)
```

**Option B: Build from Source**
```cmd
# Download GLFW source from https://www.glfw.org/
# Extract and build with CMake + Visual Studio
```

**Option C: Pre-built Binaries**
- Download from GLFW website
- Extract and note the location

### Step 2: Configure Build

#### 2.1 Open Developer Command Prompt

**Visual Studio 2019:**
- Start Menu → "x64 Native Tools Command Prompt for VS 2019"

**Visual Studio 2022:**
- Start Menu → "x64 Native Tools Command Prompt for VS 2022"

**Or use regular Command Prompt:**
```cmd
# Navigate to Visual Studio installation
cd "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build"
vcvars64.bat
```

#### 2.2 Navigate to Project

```cmd
cd C:\path\to\EnjinEngine
```

#### 2.3 Create Build Directory

```cmd
mkdir build
cd build
```

#### 2.4 Configure CMake

**If using vcpkg:**
```cmd
cmake .. ^
    -G "Visual Studio 16 2019" -A x64 ^
    -DCMAKE_TOOLCHAIN_FILE=C:\path\to\vcpkg\scripts\buildsystems\vcpkg.cmake
```

**If GLFW is installed elsewhere:**
```cmd
cmake .. ^
    -G "Visual Studio 16 2019" -A x64 ^
    -DGLFW3_DIR=C:\path\to\glfw\lib\cmake\glfw3
```

**Standard (if GLFW found automatically):**
```cmd
cmake .. -G "Visual Studio 16 2019" -A x64
```

**For Visual Studio 2022:**
```cmd
cmake .. -G "Visual Studio 17 2022" -A x64
```

### Step 3: Build

```cmd
# Build Release
cmake --build . --config Release

# Or build Debug
cmake --build . --config Debug
```

**Expected output:**
- `build\bin\Release\ExampleTriangle.exe` (if examples enabled)
- `build\lib\Release\EnjinCore.lib`
- `build\lib\Release\EnjinEngine.lib`

### Step 4: Run

```cmd
cd bin\Release
ExampleTriangle.exe
```

**Expected result:**
- Window opens
- Red triangle renders
- Logs appear in console

## 🔍 Troubleshooting

### "Cannot find Vulkan"

**Check installation:**
```cmd
vulkaninfo --summary
```

**If not found, set environment variable:**
```cmd
set VULKAN_SDK=C:\VulkanSDK\1.3.xxx.x
set PATH=%VULKAN_SDK%\Bin;%PATH%
```

**Or set permanently:**
- Control Panel → System → Advanced → Environment Variables
- Add `VULKAN_SDK` = `C:\VulkanSDK\1.3.xxx.x`
- Add `%VULKAN_SDK%\Bin` to PATH

### "Cannot find GLFW"

**If using vcpkg:**
```cmd
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\path\to\vcpkg\scripts\buildsystems\vcpkg.cmake
```

**If GLFW built manually:**
```cmd
cmake .. -DGLFW3_DIR=C:\path\to\glfw\install\lib\cmake\glfw3
```

**Check CMake output:**
- Look for "Found GLFW" message
- If not found, CMake will show what it's looking for

### "C++20 not supported"

- Ensure Visual Studio 2019 Update 16.11+ or Visual Studio 2022
- Check project properties → C/C++ → Language → C++ Language Standard

### "Linker errors"

**Common issues:**
- Architecture mismatch (x86 vs x64) - ensure everything is x64
- Missing libraries - check that Vulkan SDK is properly installed
- GLFW not linked - verify CMake found GLFW

**Solution:**
```cmd
# Clean and reconfigure
cd build
rmdir /s /q *
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
```

### "Window opens but nothing renders"

**Check:**
1. Console logs for errors
2. Vulkan validation layers (if installed)
3. GPU drivers are up to date
4. Vulkan is supported: `vulkaninfo --summary`

### Build Errors

**"Cannot open include file"**
- Check include paths in CMakeLists.txt
- Verify all source files exist

**"Unresolved external symbol"**
- Missing library link
- Check CMakeLists.txt linking section

## ✅ Verification Checklist

After building, verify:

- [ ] `ExampleTriangle.exe` exists in `build\bin\Release\`
- [ ] No linker errors during build
- [ ] Executable runs without crashing
- [ ] Window opens
- [ ] Triangle renders (red triangle)
- [ ] Console shows logs

## 🎯 What to Expect

### Successful Build:
```
-- Configuring done
-- Generating done
-- Build files have been written to: C:/path/to/build
```

### Successful Run:
```
[INFO] [Game] Triangle Example starting...
[INFO] [Renderer] Initializing RenderSystem...
[INFO] [Renderer] RenderSystem initialized
[INFO] [Game] Triangle Example initialized
```

### Window:
- Opens with title bar
- Shows red triangle in center
- Can be closed normally

## 🚀 Next Steps After It Runs

1. **Verify Basic Rendering**
   - Triangle should be visible
   - Should be red color
   - Should be centered

2. **Test ECS System**
   - Add more entities
   - Test component systems

3. **Enable GPU Culling**
   - Compile compute shader (optional)
   - Test with many objects

4. **Add Features**
   - Implement material system
   - Add physics
   - Add weather/day-night cycle

## 📚 Additional Resources

- [WINDOWS_BUILD.md](WINDOWS_BUILD.md) - Detailed Windows build guide
- [BUILD.md](BUILD.md) - General build instructions
- [GETTING_STARTED.md](GETTING_STARTED.md) - Running the engine
- [TROUBLESHOOTING.md](TROUBLESHOOTING.md) - Common issues

## 💡 Tips

1. **Use Visual Studio GUI**: Open `CMakeLists.txt` directly in Visual Studio for easier building
2. **Check CMake Output**: Look for "Found" messages to verify dependencies
3. **Build Debug First**: Easier to debug issues
4. **Keep Console Open**: Logs appear in console window
5. **Update GPU Drivers**: Ensure latest Vulkan-compatible drivers

---

**Ready to build? Start with Step 1!**
