# Enjin Engine

A proprietary, licensable 3D game engine built from scratch using Vulkan API. Designed for maximum performance, extensibility, and ease of use.

## ⚠️ Status: Foundation Complete, Implementation In Progress

**Current State**: The engine has complete architecture and interfaces, but many implementations are stubs that need completion. See [STATUS.md](docs/STATUS.md) for details.

**To Run**: You'll need to compile shaders and complete some critical implementations. See [GETTING_STARTED.md](docs/GETTING_STARTED.md) for getting started.

## 🚀 Quick Start

### 1. Check Dependencies

```bash
./scripts/check_dependencies.sh
```

### 2. Install Dependencies

**See [INSTALL_DEPENDENCIES.md](docs/INSTALL_DEPENDENCIES.md) for detailed, up-to-date instructions.**

Quick version:
```bash
# Ubuntu/Debian
sudo apt-get install build-essential cmake libvulkan-dev libglfw3-dev glslang-tools

# Or download Vulkan SDK from: https://vulkan.lunarg.com/sdk/home
```

### 3. Compile Shaders

```bash
./scripts/compile_shaders.sh
```

### 4. Build

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

### 5. Run

```bash
cd bin
./ExampleTriangle
```

## 📚 Documentation

- **[Getting Started](docs/GETTING_STARTED.md)** - Step-by-step guide
- **[Install Dependencies](docs/INSTALL_DEPENDENCIES.md)** - ⚠️ **UP-TO-DATE** dependency installation
- **[Build Instructions](docs/BUILD.md)** - Detailed build guide
- **[Minimal Setup](docs/MINIMAL_SETUP.md)** - If packages aren't available
- **[Architecture](docs/ARCHITECTURE.md)** - System architecture overview
- **[Complete System Guide](docs/COMPLETE_SYSTEM_GUIDE.md)** - How to use all systems

## 🎯 Key Features

### Rendering
- ✅ **GPU-Driven Rendering** - Cull and render 10,000-100,000 objects
- ✅ **Bindless Resources** - Single descriptor set, access by handle
- ✅ **Extensible Pipeline** - Hooks, hot-reload, scriptable
- ✅ **Multiple Techniques** - Easy switching between rendering styles
- ✅ **Hot-Reloadable** - Shaders and materials reload automatically

### Systems
- ✅ **Day/Night Cycle** - Realistic sun movement and sky colors
- ✅ **Weather System** - Rain, snow, fog effects
- ✅ **Physics Engine** - Simple rigid body dynamics
- ✅ **Water Rendering** - Realistic water with waves
- ✅ **Shader GUI** - Real-time shader parameter editing

### Architecture
- ✅ **ECS System** - Entity Component System for game objects
- ✅ **Memory Management** - Custom allocators (Stack, Pool, Linear)
- ✅ **Math Library** - SIMD-ready vectors, matrices, quaternions
- ✅ **Logging System** - Thread-safe, categorized logging

## 🛠️ Technology Stack

- **Language**: C++20
- **Graphics**: Vulkan 1.3
- **Windowing**: GLFW3
- **Build**: CMake 3.20+

## 📝 License

Proprietary - All rights reserved.

## 🗺️ Roadmap

See [TOP_TIER_ROADMAP.md](docs/TOP_TIER_ROADMAP.md) for complete development roadmap.

## 🤝 Contributing

This is a proprietary engine. For licensing inquiries, please contact the maintainers.

---

**Enjin Engine** - Building the future of game engines, one system at a time.
