# Enjin Engine Architecture Documentation

## Overview

Enjin Engine is a proprietary, licensable 3D game engine built from scratch using Vulkan API. The engine is designed for maximum performance, extensibility, and ease of use.

## Core Principles

1. **GPU-First Architecture**: Everything that can run on GPU does
2. **Bindless Everything**: No descriptor set binding overhead
3. **Extensible & Scriptable**: Easy to modify and extend
4. **Data-Driven**: Materials, assets, and configuration in files
5. **Hot-Reloadable**: See changes instantly without restarting

## System Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    Application Layer                     │
│  (Game Code, Editor, Examples)                          │
└─────────────────────────────────────────────────────────┘
                            │
┌─────────────────────────────────────────────────────────┐
│                    Engine Layer                          │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐   │
│  │   Renderer   │  │   Physics    │  │    Audio    │   │
│  │   System     │  │   System     │  │   System    │   │
│  └──────────────┘  └──────────────┘  └──────────────┘   │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐   │
│  │    ECS       │  │   Assets     │  │  Scripting   │   │
│  │   System     │  │   System     │  │   System     │   │
│  └──────────────┘  └──────────────┘  └──────────────┘   │
└─────────────────────────────────────────────────────────┘
                            │
┌─────────────────────────────────────────────────────────┐
│                    Core Layer                            │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐   │
│  │   Memory     │  │     Math     │  │   Logging    │   │
│  │  Management  │  │   Library    │  │   System     │   │
│  └──────────────┘  └──────────────┘  └──────────────┘   │
│  ┌──────────────┐  ┌──────────────┐                      │
│  │  Platform    │  │   Platform   │                      │
│  │  Abstraction │  │   Window     │                      │
│  └──────────────┘  └──────────────┘                      │
└─────────────────────────────────────────────────────────┘
                            │
┌─────────────────────────────────────────────────────────┐
│                 Vulkan API Layer                         │
│  (Instance, Device, Queues, Commands)                    │
└─────────────────────────────────────────────────────────┘
```

## Key Systems

### Rendering System

**Components**:
- `VulkanRenderer`: Main rendering interface
- `VulkanContext`: Vulkan instance, device, queues
- `VulkanSwapchain`: Swapchain management
- `VulkanPipeline`: Graphics pipeline management
- `VulkanBuffer`: Buffer management
- `VulkanShader`: Shader module management
- `RenderPipeline`: Extensible rendering pipeline with hooks
- `GPUCullingSystem`: GPU-driven frustum culling
- `BindlessResourceManager`: Bindless resource management

**Features**:
- GPU-driven rendering
- Bindless resources
- Hot-reloadable shaders
- Extensible pipeline with hooks
- Multiple rendering techniques

### ECS System

**Components**:
- `World`: Main ECS container
- `Entity`: ID-based entities
- `Component`: Data components (Transform, Mesh, etc.)
- `System`: Logic systems (RenderSystem, etc.)

**Features**:
- Structure of Arrays (SoA) for cache efficiency
- Type-safe component storage
- System-based logic
- Data-oriented design

### Physics System

**Components**:
- `PhysicsWorld`: Physics simulation world
- `RigidBody`: Rigid body physics
- `Collider`: Collision shapes
- `PhysicsMaterial`: Material properties

**Features**:
- Rigid body dynamics
- Collision detection
- Simple and performant

### Weather System

**Components**:
- `WeatherManager`: Weather state management
- `ParticleSystem`: Weather particles (rain, snow)
- `SkySystem`: Sky rendering and atmosphere

**Features**:
- Rain, snow, fog effects
- Dynamic weather transitions
- Performance-optimized particles

### Day/Night Cycle

**Components**:
- `TimeOfDay`: Time management
- `SunSystem`: Sun position and lighting
- `SkyRenderer`: Sky dome rendering

**Features**:
- Realistic sun movement
- Dynamic sky colors
- Time-based lighting

### Water System

**Components**:
- `WaterRenderer`: Water surface rendering
- `WaterSimulation`: Wave simulation
- `UnderwaterEffects`: Underwater rendering

**Features**:
- Realistic water rendering
- Wave simulation
- Reflection/refraction
- Underwater effects

## File Structure

```
Engine/
├── Core/              # Foundation (Memory, Math, Logging, Platform)
├── Engine/            # Engine systems (Renderer, ECS, Physics, etc.)
│   ├── Renderer/      # Rendering systems
│   ├── ECS/           # Entity Component System
│   ├── Physics/       # Physics engine
│   ├── Weather/       # Weather system
│   ├── Time/          # Day/night cycle
│   ├── Water/         # Water rendering
│   └── GUI/           # GUI system
├── Editor/            # Editor application
└── Examples/          # Example projects
```

## Design Patterns

### Component-Based Architecture
- Entities are IDs
- Components are data
- Systems contain logic
- Enables data-oriented design

### Plugin System
- Rendering techniques as plugins
- Hot-reloadable systems
- Extensible architecture

### Data-Driven Design
- Materials in JSON
- Configs in files
- Hot-reloadable assets

## Performance Considerations

### Memory Management
- Custom allocators (Stack, Pool, Linear)
- No memory fragmentation
- Cache-friendly layouts

### GPU Optimization
- GPU-driven rendering
- Bindless resources
- Parallel processing
- Minimal CPU overhead

### Scalability
- Support 10,000-100,000 objects
- Efficient culling
- LOD systems
- Streaming

## Extension Points

### Rendering Techniques
- Register new techniques
- Switch at runtime
- Custom pipelines

### Shader System
- Hot-reloadable shaders
- Material system
- Shader generation

### Scripting
- Lua/Python integration
- Runtime modification
- Hot-reload support

## Documentation Standards

All code should include:
- Header comments explaining purpose
- Parameter documentation
- Usage examples
- Performance notes
- Thread safety notes

## Thread Safety

- Rendering: Single-threaded (GPU-driven)
- ECS: Thread-safe component access
- Physics: Can run in separate thread
- Asset loading: Async, thread-safe

## Memory Safety

- RAII for all resources
- Smart pointers for ownership
- No raw pointers in public API
- Automatic cleanup

## Error Handling

- Logging for all errors
- Graceful degradation
- Fallback systems
- Clear error messages
