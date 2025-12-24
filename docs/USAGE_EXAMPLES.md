# Extensible Rendering - Usage Examples

## Example 1: Runtime Graphics Tweaking

```cpp
#include "Enjin/Renderer/RenderPipeline/RenderPipeline.h"

// Get the render pipeline
RenderPipeline* pipeline = GetRenderPipeline();

// Tweak pipeline state in real-time
RenderPipeline::PipelineState state = pipeline->GetPipelineState();
state.lineWidth = 2.0f;
state.polygonMode = VK_POLYGON_MODE_LINE; // Wireframe mode
state.depthTest = false;
pipeline->SetPipelineState(state);

// Modify material parameters
RenderPipeline::Material* material = pipeline->GetMaterial("PBR_Material");
if (material) {
    material->floatParams["metallic"] = 0.8f;
    material->floatParams["roughness"] = 0.2f;
    material->vectorParams["baseColor"] = Math::Vector4(1.0f, 0.5f, 0.0f, 1.0f);
}
```

## Example 2: Custom Rendering Hook

```cpp
// Add custom post-processing effect
pipeline->RegisterHook(
    RenderEventType::PostRenderPass,
    [](RenderEvent& event) {
        // Custom post-processing
        ApplyBloomEffect();
        ApplyToneMapping();
        ApplyColorGrading();
    },
    "PostProcessing"
);

// Add custom pre-draw hook
pipeline->RegisterHook(
    RenderEventType::PreDraw,
    [](RenderEvent& event) {
        // Modify draw parameters
        u32* indexCount = static_cast<u32*>(event.data);
        // Could modify indexCount here
    },
    "CustomDrawModifier"
);
```

## Example 3: Material Hot-Reload

```cpp
// Load material
u32 materialId = pipeline->RegisterMaterial({
    .name = "TestMaterial",
    .shaderPath = "shaders/test.frag",
    .floatParams = {{"metallic", 0.5f}},
    .vectorParams = {{"baseColor", Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f)}}
});

// Later, reload material (e.g., when file changes)
pipeline->ReloadMaterial(materialId);

// Or reload all materials
pipeline->ReloadAllShaders();
```

## Example 4: Script Integration

```cpp
// Register script callbacks
pipeline->RegisterScriptCallback("OnFrameStart", []() {
    // Custom frame start logic
});

pipeline->RegisterScriptCallback("ApplyPostProcessing", []() {
    // Post-processing from script
});

// Call from script or code
pipeline->CallScriptCallback("OnFrameStart");
```

## Example 5: Material Override Hook

```cpp
// Override material selection
pipeline->RegisterHook(
    RenderEventType::MaterialOverride,
    [](RenderEvent& event) {
        RenderPipeline::Material* material = 
            static_cast<RenderPipeline::Material*>(event.data);
        
        // Override material based on conditions
        if (someCondition) {
            material->floatParams["emissive"] = 1.0f;
            material->vectorParams["emissiveColor"] = 
                Math::Vector4(1.0f, 0.0f, 0.0f, 1.0f);
        }
    },
    "MaterialOverride"
);
```

## Example 6: Debug Visualization

```cpp
// Enable debug visualization
pipeline->EnableDebugVisualization(true);

// Add debug hooks
pipeline->RegisterHook(
    RenderEventType::PreDraw,
    [](RenderEvent& event) {
        // Draw bounding boxes, normals, etc.
        DrawDebugVisualization();
    },
    "DebugViz"
);
```

## Example 7: Conditional Rendering

```cpp
// Skip rendering under certain conditions
pipeline->RegisterHook(
    RenderEventType::PreDraw,
    [](RenderEvent& event) {
        if (shouldSkipRendering) {
            event.cancelled = true; // Skip this draw
        }
    },
    "ConditionalRender"
);
```

## Example 8: Performance Monitoring

```cpp
// Monitor draw calls
u32 drawCallCount = 0;
pipeline->RegisterHook(
    RenderEventType::PreDraw,
    [&drawCallCount](RenderEvent& event) {
        drawCallCount++;
    },
    "DrawCallCounter"
);

// Monitor frame time
f32 frameTime = 0.0f;
pipeline->RegisterHook(
    RenderEventType::PreFrame,
    [&frameTime](RenderEvent& event) {
        auto start = std::chrono::high_resolution_clock::now();
        // ... render ...
        auto end = std::chrono::high_resolution_clock::now();
        frameTime = std::chrono::duration<f32>(end - start).count();
    },
    "FrameTimer"
);
```

## Example 9: Integration with RenderSystem

```cpp
// In RenderSystem
class RenderSystem : public ISystem {
    RenderPipeline* m_Pipeline;
    
    void RenderEntity(Entity entity) {
        // Use pipeline instead of direct Vulkan calls
        m_Pipeline->BeginRenderPass(renderPass, framebuffer);
        m_Pipeline->BindPipeline(pipeline);
        m_Pipeline->Draw(commandBuffer, indexCount);
        m_Pipeline->EndRenderPass();
    }
};
```

## Example 10: File-Based Material System

```json
// materials/pbr.json
{
  "name": "PBR_Material",
  "vertexShader": "shaders/pbr.vert",
  "fragmentShader": "shaders/pbr.frag",
  "parameters": {
    "metallic": 0.5,
    "roughness": 0.3,
    "ao": 1.0
  },
  "vectorParams": {
    "baseColor": [1.0, 0.8, 0.6, 1.0]
  },
  "textures": {
    "albedo": "textures/wood_albedo.png",
    "normal": "textures/wood_normal.png",
    "roughness": "textures/wood_roughness.png"
  },
  "pipelineState": {
    "depthTest": true,
    "cullMode": "back"
  }
}
```

```cpp
// Load material from file
MaterialSystem* materials = GetMaterialSystem();
u32 materialId = materials->LoadMaterial("materials/pbr.json");

// Material automatically hot-reloads when file changes!
```

## Key Benefits

1. **Easy Tweaking**: Change materials/parameters without recompiling
2. **Hot-Reload**: See changes instantly
3. **Scriptable**: Modify behavior via scripts
4. **Extensible**: Add custom rendering easily
5. **Future-Proof**: Ready for visual scripting, plugins, etc.

All rendering is now **accessible**, **extensible**, and **hot-reloadable**!
