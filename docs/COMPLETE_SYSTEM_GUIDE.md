# Complete System Guide

## Overview

This guide covers all major systems in Enjin Engine and how to use them together.

## System Integration

### Initialization Order

```cpp
// 1. Initialize core systems
Logger::Get().Initialize();
Window* window = CreateWindow({1280, 720, "Enjin Engine"});

// 2. Initialize renderer
VulkanRenderer renderer;
renderer.Initialize(window);

RenderPipeline pipeline(&renderer);
pipeline.Initialize();

// 3. Initialize rendering techniques
RenderingTechniqueManager techniques;
techniques.RegisterTechnique(std::make_unique<ForwardRendering>());
techniques.SwitchTechnique("ForwardRendering");
techniques.InitializeCurrent(&renderer, &pipeline);

// 4. Initialize game systems
TimeOfDay timeOfDay;
timeOfDay.SetTime(12.0f); // Noon
timeOfDay.SetDayLength(300.0f); // 5 minutes = 24 hours

WeatherSystem weather;
weather.Initialize();
weather.SetWeather(WeatherType::Clear);

PhysicsWorld physics;
physics.Initialize();
physics.SetGravity(Math::Vector3(0, -9.81f, 0));

WaterRenderer water;
water.Initialize(&renderer);
water.SetWaterLevel(0.0f);

// 5. Initialize GUI
ShaderGUI shaderGUI;
shaderGUI.Initialize();
```

### Main Loop

```cpp
while (running) {
    f32 deltaTime = GetDeltaTime();
    
    // Update systems
    timeOfDay.Update(deltaTime);
    weather.Update(deltaTime);
    physics.Step(deltaTime);
    
    // Render
    renderer.BeginFrame();
    
    // Update lighting based on time of day
    UpdateLighting(timeOfDay);
    
    // Render with current technique
    techniques.Render(deltaTime);
    
    // Render water
    water.Render(deltaTime, cameraPosition);
    
    // Render GUI
    shaderGUI.Render();
    
    renderer.EndFrame();
}
```

## Rendering Techniques

### Switching Techniques

```cpp
// Register techniques
techniques.RegisterTechnique(std::make_unique<ForwardRendering>());
techniques.RegisterTechnique(std::make_unique<DeferredRendering>());
techniques.RegisterTechnique(std::make_unique<ClusteredForward>());

// Switch at runtime
techniques.SwitchTechnique("DeferredRendering");

// Get current technique
RenderingTechnique* current = techniques.GetCurrentTechnique();
```

### Creating Custom Techniques

```cpp
class MyCustomTechnique : public RenderingTechnique {
public:
    MyCustomTechnique() : RenderingTechnique("MyCustom") {}
    
    bool Initialize(VulkanRenderer* renderer, RenderPipeline* pipeline) override {
        // Setup custom rendering
        return true;
    }
    
    void Render(f32 deltaTime) override {
        // Custom rendering logic
    }
};

// Register
techniques.RegisterTechnique(std::make_unique<MyCustomTechnique>());
```

## Day/Night Cycle

### Basic Usage

```cpp
TimeOfDay time;

// Set time
time.SetTime(6.0f);  // 6 AM (sunrise)
time.SetTime(12.0f); // Noon
time.SetTime(18.0f); // 6 PM (sunset)
time.SetTime(0.0f);  // Midnight

// Set day length (real-world seconds)
time.SetDayLength(300.0f); // 5 minutes = 24 hours game time

// Update
time.Update(deltaTime);

// Get lighting values
Math::Vector3 sunDir = time.GetSunDirection();
Math::Vector3 sunColor = time.GetSunColor();
Math::Vector4 skyColor = time.GetSkyColor();
f32 sunIntensity = time.GetSunIntensity();
```

### Integration with Lighting

```cpp
void UpdateLighting(const TimeOfDay& time) {
    // Update directional light (sun)
    DirectionalLight sun;
    sun.direction = time.GetSunDirection();
    sun.color = time.GetSunColor();
    sun.intensity = time.GetSunIntensity();
    
    // Update ambient light
    AmbientLight ambient;
    ambient.color = time.GetAmbientColor();
    
    // Update sky
    SkyRenderer::SetSkyColor(time.GetSkyColor());
}
```

## Weather System

### Basic Usage

```cpp
WeatherSystem weather;
weather.Initialize();

// Set weather
weather.SetWeather(WeatherType::Rain, 0.8f); // Heavy rain
weather.SetWeather(WeatherType::Snow, 0.5f); // Light snow
weather.SetWeather(WeatherType::Fog, 1.0f);   // Dense fog
weather.SetWeather(WeatherType::Clear, 0.0f); // Clear sky

// Set wind
weather.SetWindDirection(Math::Vector3(1.0f, 0.0f, 0.5f));
weather.SetWindSpeed(5.0f);

// Update
weather.Update(deltaTime);

// Get fog parameters
f32 fogDensity = weather.GetFogDensity();
Math::Vector3 fogColor = weather.GetFogColor();
```

### Weather Transitions

```cpp
// Smoothly transition weather
void TransitionWeather(WeatherSystem& weather, WeatherType target, f32 duration) {
    // Weather system automatically transitions intensity
    weather.SetWeather(target, 1.0f);
    // Transition happens over time in Update()
}
```

## Physics System

### Basic Usage

```cpp
PhysicsWorld physics;
physics.Initialize();
physics.SetGravity(Math::Vector3(0, -9.81f, 0));

// Create rigid body
auto body = std::make_shared<RigidBody>();
body->SetPosition(Math::Vector3(0, 10, 0));
body->SetMass(1.0f);
body->SetStatic(false);

// Add to world
physics.AddRigidBody(body);

// Step simulation
physics.Step(deltaTime);

// Get updated position
Math::Vector3 pos = body->GetPosition();
```

## Water System

### Basic Usage

```cpp
WaterRenderer water;
water.Initialize(&renderer);

// Set water level
water.SetWaterLevel(0.0f);

// Set wave parameters
water.SetWaveAmplitude(0.5f);
water.SetWaveFrequency(1.0f);
water.SetWaveSpeed(1.0f);

// Render
water.Render(deltaTime, cameraPosition);
```

## Shader GUI

### Basic Usage

```cpp
ShaderGUI gui;
gui.Initialize();

// Register materials for editing
gui.RegisterMaterial(materialInstance);

// Show/hide editors
gui.ShowMaterialEditor(true);
gui.ShowShaderEditor(true);

// Render GUI (in render loop)
gui.Render();
```

## Complete Example

```cpp
class GameApplication : public Application {
    VulkanRenderer renderer;
    RenderPipeline pipeline;
    RenderingTechniqueManager techniques;
    TimeOfDay timeOfDay;
    WeatherSystem weather;
    PhysicsWorld physics;
    WaterRenderer water;
    ShaderGUI gui;
    
    void Initialize() override {
        // Initialize all systems
        renderer.Initialize(GetWindow());
        pipeline.Initialize();
        
        techniques.RegisterTechnique(std::make_unique<ForwardRendering>());
        techniques.SwitchTechnique("ForwardRendering");
        techniques.InitializeCurrent(&renderer, &pipeline);
        
        timeOfDay.SetTime(12.0f);
        weather.Initialize();
        physics.Initialize();
        water.Initialize(&renderer);
        gui.Initialize();
    }
    
    void Update(f32 deltaTime) override {
        timeOfDay.Update(deltaTime);
        weather.Update(deltaTime);
        physics.Step(deltaTime);
    }
    
    void Render() override {
        renderer.BeginFrame();
        
        // Update lighting
        UpdateLighting(timeOfDay);
        
        // Render
        techniques.Render(deltaTime);
        water.Render(deltaTime, cameraPosition);
        gui.Render();
        
        renderer.EndFrame();
    }
};
```

## System Dependencies

```
Application
    ├── Renderer (VulkanRenderer)
    │   ├── RenderPipeline
    │   │   └── RenderingTechniqueManager
    │   ├── GPU Culling
    │   └── Bindless Resources
    ├── TimeOfDay
    │   └── Lighting System
    ├── WeatherSystem
    │   └── Particle Systems
    ├── PhysicsWorld
    │   └── Rigid Bodies
    ├── WaterRenderer
    │   └── Renderer
    └── ShaderGUI
        └── MaterialSystem
```

All systems are designed to work together seamlessly!
