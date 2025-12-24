# Extensible Rendering Pipeline Design

## 🎯 Design Philosophy

**Goal**: Make the rendering pipeline easily accessible for:
- Graphics tweaking
- Script injection
- Hot-reloading
- Custom rendering effects
- Future extensibility

## 🏗️ Architecture

### 1. Render Pipeline with Hooks

**RenderPipeline** - Central extensible rendering system

```cpp
RenderPipeline pipeline(renderer);

// Register hooks for any rendering event
pipeline.RegisterHook(RenderEventType::PreDraw, [](RenderEvent& event) {
    // Custom logic before every draw call
    // Can modify, cancel, or add custom behavior
});

pipeline.RegisterHook(RenderEventType::MaterialOverride, [](RenderEvent& event) {
    // Override material selection
    Material* material = static_cast<Material*>(event.data);
    // Modify material parameters
});
```

**Available Hook Points**:
- `PreFrame` / `PostFrame` - Frame boundaries
- `PreRenderPass` / `PostRenderPass` - Render pass boundaries
- `PreDraw` / `PostDraw` - Individual draw calls
- `PreShaderBind` / `PostShaderBind` - Shader binding
- `MaterialOverride` - Material selection
- `CullingOverride` - Culling decisions
- `Custom` - User-defined events

### 2. Material System (Data-Driven)

**Materials are JSON files** - Easy to edit, hot-reloadable:

```json
{
  "name": "PBR_Material",
  "vertexShader": "shaders/pbr.vert",
  "fragmentShader": "shaders/pbr.frag",
  "parameters": {
    "metallic": 0.5,
    "roughness": 0.3,
    "baseColor": [1.0, 0.8, 0.6, 1.0]
  },
  "textures": {
    "albedo": "textures/wood_albedo.png",
    "normal": "textures/wood_normal.png"
  },
  "pipelineState": {
    "depthTest": true,
    "cullMode": "back"
  }
}
```

**Hot-Reload**: Change JSON file → Material reloads automatically

### 3. Script Injection System

**RenderScript** - Execute scripts to modify rendering:

```lua
-- Example Lua script
function OnPreDraw()
    -- Modify draw parameters
    SetLineWidth(2.0)
    EnableWireframe(true)
end

function OnMaterialOverride(material)
    -- Override material
    material.parameters.metallic = 0.8
end

-- Register hooks from script
RegisterHook("PreDraw", "OnPreDraw")
RegisterHook("MaterialOverride", "OnMaterialOverride")
```

### 4. Shader Hot-Reload

**Automatic shader reloading** when files change:

```cpp
ShaderHotReload hotReload;
hotReload.RegisterShader("shaders/test.frag", fragmentShader);
hotReload.SetEnabled(true);

// In render loop:
hotReload.Update(); // Checks for changes and reloads
```

## 📝 Usage Examples

### Example 1: Runtime Graphics Tweaking

```cpp
// In-game graphics settings
RenderPipeline* pipeline = GetRenderPipeline();

// Tweak pipeline state
PipelineState state = pipeline->GetPipelineState();
state.lineWidth = 2.0f;
state.polygonMode = VK_POLYGON_MODE_LINE; // Wireframe
pipeline->SetPipelineState(state);

// Modify material parameters
Material* material = pipeline->GetMaterial("PBR_Material");
material->floatParams["metallic"] = 0.8f;
material->vectorParams["baseColor"] = Vector4(1.0f, 0.5f, 0.0f, 1.0f);
```

### Example 2: Custom Rendering Effect Hook

```cpp
// Add custom post-processing effect
pipeline->RegisterHook(RenderEventType::PostRenderPass, 
    [](RenderEvent& event) {
        // Custom post-processing
        ApplyBloomEffect();
        ApplyToneMapping();
    }, 
    "PostProcessing"
);
```

### Example 3: Script-Based Material Tweaking

```cpp
// Load and execute script
RenderScript script(&pipeline);
script.ExecuteFile("scripts/graphics_tweaks.lua");

// Script can modify materials, pipeline state, etc.
// All changes are hot-reloaded
```

### Example 4: Material Hot-Reload

```cpp
// Material file changes → automatically reloads
MaterialSystem* materials = GetMaterialSystem();
u32 materialId = materials->LoadMaterial("materials/pbr.json");

// Edit materials/pbr.json in external editor
// Material automatically reloads on next frame
```

## 🔧 Integration Points

### For Graphics Programmers:
```cpp
// Easy access to pipeline
RenderPipeline* pipeline = GetRenderPipeline();

// Modify any aspect
pipeline->SetPipelineState(state);
pipeline->RegisterHook(eventType, customHook);
```

### For Scripters/Designers:
```lua
-- Lua/Python scripts
SetMaterialParam("PBR_Material", "metallic", 0.8)
EnableWireframe(true)
RegisterHook("PreDraw", customFunction)
```

### For Artists:
```json
// Edit JSON material files
{
  "parameters": {
    "metallic": 0.5  // Change this → hot-reloads
  }
}
```

## 🚀 Future Extensibility

### Visual Scripting (Future):
- Node-based render graph editor
- Visual material editor
- Visual shader editor
- All hot-reloadable

### Plugin System (Future):
```cpp
// Load rendering plugins
LoadPlugin("plugins/custom_lighting.dll");
LoadPlugin("plugins/ray_tracing.dll");

// Plugins can register hooks, materials, etc.
```

### Runtime Shader Generation (Future):
```cpp
// Generate shaders at runtime
ShaderGenerator generator;
generator.AddPass("Lighting");
generator.AddPass("Shadows");
VulkanShader* shader = generator.Compile();
```

## 📊 Benefits

1. **Easy Tweaking**: Change materials/parameters without recompiling
2. **Hot-Reload**: See changes instantly
3. **Scriptable**: Modify behavior via scripts
4. **Extensible**: Add custom rendering easily
5. **Future-Proof**: Ready for visual scripting, plugins, etc.

## 🎮 Use Cases

### Development:
- Quick iteration on materials
- Test different rendering techniques
- Debug rendering issues

### Runtime:
- Graphics settings menu
- Post-processing effects
- Custom rendering modes

### Modding:
- User-created materials
- Custom shaders
- Scripted effects

## 🔄 Workflow

1. **Create Material** → Edit JSON file
2. **Write Shader** → Edit GLSL file
3. **Test** → Hot-reloads automatically
4. **Tweak** → Modify parameters in real-time
5. **Script** → Add custom behavior via scripts
6. **Hook** → Register custom hooks for advanced control

Everything is **data-driven**, **hot-reloadable**, and **scriptable**!
