# Enjin Engine

A proprietary, licensable game engine built from scratch using Vulkan API.

## Project Structure

```
EnjinEngine/
├── Core/           # Foundation layer (Memory, Math, Logging, Platform)
├── Engine/         # Engine layer (Renderer, ECS, Assets, Physics)
├── Editor/         # Editor application
├── Examples/       # Example projects
└── Tests/          # Unit tests
```

## Roadmap

### Phase 1: Foundation ✅
- [x] Memory Management (Stack, Pool, Linear allocators)
- [x] Math Library (Vectors, Matrices, Quaternions)
- [x] Logging System (Thread-safe, categorized)
- [x] Platform Abstraction Layer
- [x] Entry Point Abstraction

### Phase 2: Vulkan Renderer 🚧
- [x] Vulkan Context Initialization
- [ ] Swapchain Management
- [ ] Command Buffer System
- [ ] Resource Management (Bindless Descriptors)
- [ ] SPIR-V Shader Pipeline
- [ ] Render Graph

### Phase 3: Engine Core
- [ ] ECS (Entity Component System)
- [ ] Asset System (glTF, textures, audio)
- [ ] Physics Integration
- [ ] Input System

### Phase 4: Tooling
- [ ] Editor GUI (Dear ImGui)
- [ ] Scene Editor
- [ ] Hot-Reloading

### Phase 5: Licensable Features
- [ ] Scripting Language (C#/Lua)
- [ ] Profiling Tools
- [ ] Platform Abstraction Layer (Console support)

## Building

### Prerequisites
- CMake 3.20+
- C++20 compatible compiler (GCC 10+, Clang 12+, MSVC 2019+)
- Vulkan SDK
- GLFW3

### Build Instructions

**Linux:**
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

**Windows:**
```cmd
mkdir build && cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
```

**See [WINDOWS_BUILD.md](docs/WINDOWS_BUILD.md) for detailed Windows instructions.**

### Build Options
- `ENJIN_BUILD_EDITOR=ON` - Build the editor (default: ON)
- `ENJIN_BUILD_TESTS=OFF` - Build unit tests (default: OFF)
- `ENJIN_BUILD_EXAMPLES=OFF` - Build example projects (default: OFF)

## License

Proprietary - All rights reserved.

## Technology Stack

- **Language**: C++20
- **Graphics API**: Vulkan 1.3
- **Windowing**: GLFW3 (zlib/libpng license - permissive)
- **Build System**: CMake

## License Compatibility

All dependencies use permissive licenses compatible with proprietary licensing:
- GLFW3: zlib/libpng (permissive)
- Vulkan SDK: Apache 2.0 (permissive)
- Dear ImGui: MIT (permissive)
