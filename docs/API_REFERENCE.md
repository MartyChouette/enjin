# API Reference

## Core Systems

### Memory Management

```cpp
// Allocators
StackAllocator stack(1024 * 1024); // 1MB stack
void* ptr = stack.Allocate(256);
stack.FreeToMarker(marker);

PoolAllocator pool(sizeof(MyClass), 100); // Pool of 100 objects
MyClass* obj = static_cast<MyClass*>(pool.Allocate(sizeof(MyClass)));

LinearAllocator linear(1024 * 1024); // 1MB linear
void* data = linear.Allocate(512);
linear.Reset(); // Free all
```

### Math Library

```cpp
// Vectors
Math::Vector3 pos(1.0f, 2.0f, 3.0f);
Math::Vector3 normalized = pos.Normalized();
f32 length = pos.Length();
f32 dot = pos.Dot(other);

// Matrices
Math::Matrix4 transform = Math::Matrix4::Translation(pos) *
                          Math::Matrix4::Rotation(axis, angle) *
                          Math::Matrix4::Scale(scale);

// Quaternions
Math::Quaternion rot = Math::Quaternion::FromEuler(euler);
Math::Matrix4 rotMat = rot.ToMatrix();
```

### Logging

```cpp
ENJIN_LOG_INFO(Core, "Engine initialized");
ENJIN_LOG_WARN(Renderer, "Texture not found: %s", path);
ENJIN_LOG_ERROR(Physics, "Collision detection failed");
ENJIN_LOG_FATAL(Core, "Critical error: %s", message);
```

## Rendering Systems

### VulkanRenderer

```cpp
VulkanRenderer renderer;
renderer.Initialize(window);
renderer.BeginFrame();
// ... render ...
renderer.EndFrame();
```

### RenderPipeline

```cpp
RenderPipeline pipeline(&renderer);

// Register hooks
pipeline->RegisterHook(RenderEventType::PreDraw, [](RenderEvent& event) {
    // Custom logic
});

// Materials
u32 materialId = pipeline->RegisterMaterial({
    .name = "PBR",
    .shaderPath = "shaders/pbr.frag",
    .floatParams = {{"metallic", 0.5f}}
});

// Pipeline state
PipelineState state;
state.lineWidth = 2.0f;
pipeline->SetPipelineState(state);
```

### Rendering Techniques

```cpp
RenderingTechniqueManager techniques;
techniques.RegisterTechnique(std::make_unique<ForwardRendering>());
techniques.SwitchTechnique("ForwardRendering");
techniques.Render(deltaTime);
```

### GPU Culling

```cpp
GPUCullingSystem culling(context);
culling.Initialize();

std::vector<CullableObject> objects;
culling.SubmitObjects(objects);

VkBuffer indirectBuffer;
u32 drawCount;
culling.ExecuteCulling(view, proj, cmd, indirectBuffer, drawCount);
```

### Bindless Resources

```cpp
BindlessResourceManager bindless(context);
bindless.Initialize();

BindlessHandle handle = bindless.RegisterTexture(imageView, sampler);
bindless.UpdateDescriptorSet();

// In shader: texture(textures[handle], uv)
```

## ECS System

```cpp
World world;

// Create entity
Entity entity = world.CreateEntity();

// Add components
TransformComponent& transform = world.AddComponent<TransformComponent>(entity);
transform.position = Math::Vector3(0, 0, 0);

MeshComponent& mesh = world.AddComponent<MeshComponent>(entity);
mesh.vertices = { /* ... */ };

// Register systems
RenderSystem* render = world.RegisterSystem<RenderSystem>(&world, &renderer);

// Update
world.Update(deltaTime);
```

## Time System

```cpp
TimeOfDay time;
time.SetTime(12.0f); // Noon
time.SetDayLength(300.0f); // 5 min = 24 hours
time.Update(deltaTime);

Math::Vector3 sunDir = time.GetSunDirection();
Math::Vector3 sunColor = time.GetSunColor();
Math::Vector4 skyColor = time.GetSkyColor();
```

## Weather System

```cpp
WeatherSystem weather;
weather.Initialize();
weather.SetWeather(WeatherType::Rain, 0.8f);
weather.SetWindSpeed(5.0f);
weather.Update(deltaTime);

f32 fogDensity = weather.GetFogDensity();
```

## Physics System

```cpp
PhysicsWorld physics;
physics.Initialize();
physics.SetGravity(Math::Vector3(0, -9.81f, 0));

auto body = std::make_shared<RigidBody>();
body->SetPosition(Math::Vector3(0, 10, 0));
physics.AddRigidBody(body);
physics.Step(deltaTime);
```

## Water System

```cpp
WaterRenderer water;
water.Initialize(&renderer);
water.SetWaterLevel(0.0f);
water.SetWaveAmplitude(0.5f);
water.Render(deltaTime, cameraPosition);
```

## GUI System

```cpp
ShaderGUI gui;
gui.Initialize();
gui.RegisterMaterial(material);
gui.ShowMaterialEditor(true);
gui.Render();
```

## Complete API Documentation

See individual header files for detailed API documentation:
- All public methods are documented
- Parameters explained
- Usage examples included
- Performance notes provided
