# Enjin Engine Development Roadmap

This document outlines the complete roadmap for building Enjin Engine, a proprietary, licensable game engine.

## Phase 1: Foundation ✅ COMPLETE

### Memory Management
- ✅ Custom allocators (Stack, Pool, Linear)
- ✅ Memory alignment utilities
- ✅ Global allocator override

### Math Library
- ✅ Vector2, Vector3, Vector4
- ✅ Matrix4 (column-major for Vulkan)
- ✅ Quaternion
- ✅ Math utilities (Lerp, Clamp, etc.)

### Logging System
- ✅ Thread-safe logging
- ✅ Category-based filtering
- ✅ File and console output
- ✅ Log levels (Trace, Debug, Info, Warn, Error, Fatal)

### Platform Abstraction
- ✅ Platform detection macros
- ✅ Compiler detection
- ✅ Entry point abstraction

## Phase 2: Vulkan Renderer 🚧 IN PROGRESS

### Core Vulkan Setup
- ✅ Vulkan instance creation
- ✅ Physical device selection
- ✅ Logical device creation
- ✅ Queue management
- ⏳ Debug messenger (validation layers)

### Swapchain Management
- ⏳ Swapchain creation
- ⏳ Image views
- ⏳ Present mode selection
- ⏳ Surface format selection
- ⏳ Resize handling

### Command Buffers
- ⏳ Command pool management
- ⏳ Command buffer recording
- ⏳ Multi-threaded command recording
- ⏳ Command buffer submission

### Resource Management
- ⏳ Buffer management (VBO, IBO, UBO)
- ⏳ Image/texture management
- ⏳ Bindless descriptor sets
- ⏳ Descriptor pool management
- ⏳ Resource lifetime tracking

### Shader Pipeline
- ⏳ SPIR-V compilation (shaderc integration)
- ⏳ Shader module management
- ⏳ Pipeline creation (graphics, compute)
- ⏳ Pipeline cache
- ⏳ Hot-reload support

### Render Graph
- ⏳ Render pass abstraction
- ⏳ Dependency graph
- ⏳ Automatic pass ordering
- ⏳ Resource barriers
- ⏳ Frame graph execution

## Phase 3: Engine Core

### ECS (Entity Component System)
- ⏳ Entity ID system
- ⏳ Component storage (SoA - Structure of Arrays)
- ⏳ System execution
- ⏳ Query system
- ⏳ Archetype-based storage

### Asset System
- ⏳ Asset loader interface
- ⏳ glTF loader
- ⏳ Texture loader (stb_image integration)
- ⏳ Audio loader
- ⏳ Asset package format
- ⏳ Async loading
- ⏳ Asset hot-reload

### Physics
- ⏳ Decision: Write custom or license?
  - Option A: License Jolt Physics (MIT) - recommended
  - Option B: Write custom physics engine (massive undertaking)
- ⏳ Collision detection
- ⏳ Rigid body dynamics
- ⏳ Physics world management

### Input System
- ⏳ Keyboard input
- ⏳ Mouse input
- ⏳ Gamepad input
- ⏳ Input mapping system
- ⏳ Event system

## Phase 4: Tooling (Editor)

### GUI Framework
- ⏳ Dear ImGui integration
- ⏳ Custom ImGui widgets
- ⏳ Docking system

### Scene Editor
- ⏳ Viewport rendering
- ⏳ Gizmo system (translate, rotate, scale)
- ⏳ Entity selection
- ⏳ Component property editor
- ⏳ Scene hierarchy view
- ⏳ Inspector panel

### Hot-Reloading
- ⏳ Shader hot-reload
- ⏳ DLL reloading for game code
- ⏳ Asset hot-reload
- ⏳ Live editing

## Phase 5: Licensable Features

### Scripting
- ⏳ Decision: C# (Mono), Lua, or custom?
- ⏳ Script runtime
- ⏳ Bindings generator
- ⏳ Visual scripting (optional)

### Profiling Tools
- ⏳ CPU profiler
- ⏳ GPU profiler (Vulkan timing queries)
- ⏳ Memory profiler
- ⏳ Frame time graph
- ⏳ Performance overlay

### Platform Abstraction Layer (PAL)
- ⏳ Windows support
- ⏳ Linux support
- ⏳ macOS support (via MoltenVK)
- ⏳ Android support
- ⏳ Console support (PlayStation/Xbox - requires NDAs)

## Next Steps

1. **Complete Vulkan Swapchain**: Integrate GLFW windowing with Vulkan surface
2. **First Triangle**: Render a triangle to verify the pipeline works
3. **Command Buffer System**: Implement multi-threaded command recording
4. **Basic ECS**: Start with simple entity/component system
5. **Asset Loading**: Begin with glTF mesh loading

## Estimated Timeline

- **Phase 1**: ✅ Complete
- **Phase 2**: 3-6 months (Vulkan is complex)
- **Phase 3**: 6-12 months (ECS + Assets)
- **Phase 4**: 6-12 months (Editor is 50% of work)
- **Phase 5**: Ongoing (features for licensing)

**Total**: 2-3 years for a basic licensable engine, 5+ years for a competitive engine.

## Critical Decisions Needed

1. **Physics Engine**: Custom vs. Licensed (Jolt recommended)
2. **Scripting Language**: C# vs. Lua vs. Custom
3. **Target Platforms**: PC only vs. Mobile vs. Consoles
4. **Rendering Features**: PBR, Deferred rendering, Forward+, etc.
