# Getting Started - Making Enjin Engine Run

## Step-by-Step Guide

### Step 1: Install Dependencies

**⚠️ IMPORTANT: See [INSTALL_DEPENDENCIES.md](INSTALL_DEPENDENCIES.md) for detailed, up-to-date instructions!**

Quick check:
```bash
./scripts/check_dependencies.sh
```

Quick install (Ubuntu/Debian):
```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    libvulkan-dev \
    libglfw3-dev \
    glslang-tools
```

**If packages aren't available**, see:
- [INSTALL_DEPENDENCIES.md](INSTALL_DEPENDENCIES.md) - Complete guide with alternatives
- [MINIMAL_SETUP.md](MINIMAL_SETUP.md) - If you can't install everything

### Step 2: Compile Shaders

```bash
# Use the provided script
./scripts/compile_shaders.sh

# Or manually:
cd Engine/shaders
glslc triangle.vert -o triangle.vert.spv
glslc triangle.frag -o triangle.frag.spv
glslc cull.comp -o cull.comp.spv
```

### Step 3: Build the Engine

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

### Step 4: Run

```bash
# Make sure shaders are in the right place
# The engine looks for shaders relative to the executable
cd bin
./EnjinEditor
```

## Expected Output

If everything works:
- ✅ Window opens
- ✅ Vulkan initializes
- ✅ Swapchain created
- ✅ Red triangle renders

## Troubleshooting

### "Shader file not found"
- Make sure `.spv` files are in `Engine/shaders/` directory
- Or copy them to the same directory as the executable

### "Vulkan not found"
- Install Vulkan SDK
- Set `VULKAN_SDK` environment variable

### "GLFW not found"
- Install GLFW3 development package
- Or build from source

### Build Errors
- Check C++ compiler is C++20 compatible
- Ensure all dependencies are installed
- Check CMake version (3.20+)

## What Works Now

After completing these steps:
- ✅ Window creation
- ✅ Vulkan initialization
- ✅ Basic triangle rendering
- ✅ ECS system
- ✅ Day/night cycle (time calculations)
- ⚠️ GPU culling (CPU fallback if shader not compiled)
- ⚠️ Other systems (interfaces ready, implementations pending)

## Next Steps After It Runs

1. **Verify Triangle Renders** - Should see red triangle
2. **Add More Objects** - Test ECS system
3. **Test Day/Night** - See time affect lighting
4. **Compile Compute Shader** - Enable GPU culling
5. **Add Features** - Implement weather, physics, etc.

The foundation is ready - now make it run!
