# Integration Guide: Bindless + GPU Culling

## Quick Start

### 1. Compile Compute Shader

```bash
cd Engine/shaders
glslc cull.comp -o cull.comp.spv
```

### 2. Initialize Systems

```cpp
// In your renderer initialization
VulkanContext* context = renderer->GetContext();

// Initialize bindless resources
BindlessResourceManager bindless(context);
bindless.Initialize();

// Initialize GPU culling
GPUCullingSystem culling(context);
culling.Initialize();
```

### 3. Register Resources (Bindless)

```cpp
// Register textures
BindlessHandle albedoHandle = bindless.RegisterTexture(albedoImageView, sampler);
BindlessHandle normalHandle = bindless.RegisterTexture(normalImageView, sampler);

// Update descriptor set
bindless.UpdateDescriptorSet();
```

### 4. Submit Objects for Culling

```cpp
// Prepare objects
std::vector<CullableObject> objects;
for (auto& entity : entities) {
    CullableObject obj;
    obj.bounds = CalculateBounds(entity);
    obj.transform = GetTransform(entity);
    obj.meshIndex = GetMeshIndex(entity);
    obj.indexCount = GetIndexCount(entity);
    objects.push_back(obj);
}

// Submit
culling.SubmitObjects(objects);
```

### 5. Execute Culling and Render

```cpp
// In render loop
VkCommandBuffer cmd = renderer->GetCurrentCommandBuffer();

// Bind bindless descriptor set (once per frame)
vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
    pipelineLayout, 0, 1, &bindless.GetDescriptorSet(), 0, nullptr);

// Execute GPU culling
VkBuffer indirectBuffer;
u32 drawCount;
culling.ExecuteCulling(viewMatrix, projMatrix, cmd, indirectBuffer, drawCount);

// Render using indirect draws
vkCmdDrawIndexedIndirect(cmd, indirectBuffer, 0, drawCount,
    sizeof(VkDrawIndexedIndirectCommand));
```

## Shader Updates

### Update Vertex Shader for Bindless

```glsl
#version 450
#extension GL_EXT_nonuniform_qualifier : require

layout(set = 0, binding = 0) uniform sampler2D textures[]; // Bindless

layout(push_constant) uniform PushConstants {
    uint textureHandle;
} pc;

void main() {
    vec4 color = texture(textures[pc.textureHandle], uv);
}
```

## Performance Tips

1. **Bindless**: Update descriptor set only when resources change
2. **GPU Culling**: Submit objects once, execute culling every frame
3. **Indirect Draws**: Use for all culled objects
4. **Memory Barriers**: Ensure proper synchronization

## Expected Performance

- **Bindless**: 10-100x reduction in descriptor set binds
- **GPU Culling**: 10-100x more objects renderable
- **Combined**: Massive scalability improvement!
