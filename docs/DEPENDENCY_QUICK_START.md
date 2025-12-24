# Dependency Installation - Quick Start

## ⚠️ Having Trouble Installing Packages?

**Run this first:**
```bash
./scripts/check_dependencies.sh
```

This will tell you exactly what's installed and what's missing.

## If Packages Aren't Available

### Option 1: Try Alternative Package Names

```bash
# Instead of libglfw3-dev, try:
sudo apt-get install libglfw-dev

# Instead of glslang-tools, try:
sudo apt-get install glslang-dev
```

### Option 2: Build from Source

See [INSTALL_DEPENDENCIES.md](INSTALL_DEPENDENCIES.md) for instructions on building:
- GLFW from source
- Vulkan SDK manually
- glslang from source

### Option 3: Use Minimal Setup

See [MINIMAL_SETUP.md](MINIMAL_SETUP.md) - engine will work with reduced features!

## What You Actually Need

**Minimum to compile:**
- ✅ C++ compiler (g++)
- ✅ CMake
- ✅ Vulkan headers (libvulkan-dev OR Vulkan SDK)
- ✅ GLFW headers (libglfw3-dev OR build from source)

**Optional (for shader compilation):**
- glslc (from glslang-tools OR Vulkan SDK)

**The engine will work without glslc** - GPU culling will use CPU fallback.

## Still Stuck?

1. Check your OS version: `cat /etc/os-release`
2. Check available packages: `apt-cache search vulkan`
3. See [INSTALL_DEPENDENCIES.md](INSTALL_DEPENDENCIES.md) for your specific OS
4. Try [MINIMAL_SETUP.md](MINIMAL_SETUP.md) for workarounds

