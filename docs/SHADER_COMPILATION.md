# Shader Compilation Guide

## Current Status

The engine includes pre-compiled SPIR-V shaders for the triangle example. However, for production use, you'll want to compile GLSL shaders to SPIR-V.

## Option 1: Use glslc (Recommended)

`glslc` is part of the Vulkan SDK and can compile GLSL to SPIR-V:

```bash
# Compile vertex shader
glslc shader.vert -o shader.vert.spv

# Compile fragment shader
glslc shader.frag -o shader.frag.spv
```

Then load them in code:
```cpp
VulkanShader shader(context);
shader.LoadFromFile("shader.vert.spv");
```

## Option 2: Integrate shaderc

The `shaderc` library (Google's shader compiler) can be integrated for runtime compilation:

1. Add shaderc to CMakeLists.txt:
```cmake
find_package(shaderc REQUIRED)
target_link_libraries(EnjinEngine PUBLIC shaderc::shaderc)
```

2. Implement `ShaderCompiler::CompileGLSL()` in `VulkanShader.cpp`:
```cpp
#include <shaderc/shaderc.hpp>

bool CompileGLSL(const std::string& source, VkShaderStageFlagBits stage, std::vector<u32>& spirv) {
    shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    
    shaderc_shader_kind kind = (stage == VK_SHADER_STAGE_VERTEX_BIT) 
        ? shaderc_vertex_shader 
        : shaderc_fragment_shader;
    
    shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(
        source, kind, "shader", options);
    
    if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
        ENJIN_LOG_ERROR(Renderer, "Shader compilation failed: %s", 
            result.GetErrorMessage().c_str());
        return false;
    }
    
    spirv = {result.cbegin(), result.cend()};
    return true;
}
```

## Option 3: Pre-compile at Build Time

Use CMake to compile shaders during build:

```cmake
# Find glslc
find_program(GLSLC glslc)

# Compile shaders
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/shaders/triangle.vert.spv
    COMMAND ${GLSLC} ${CMAKE_SOURCE_DIR}/shaders/triangle.vert -o ${CMAKE_BINARY_DIR}/shaders/triangle.vert.spv
    DEPENDS ${CMAKE_SOURCE_DIR}/shaders/triangle.vert
)

# Copy to output directory
file(COPY ${CMAKE_BINARY_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR}/bin)
```

## Shader Location

Place your GLSL shaders in:
```
Engine/shaders/
    triangle.vert
    triangle.frag
```

## Example Shaders

### triangle.vert
```glsl
#version 450
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    fragColor = vec3(1.0, 0.0, 0.0); // Red color
}
```

### triangle.frag
```glsl
#version 450
layout(location = 0) in vec3 fragColor;
layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(fragColor, 1.0);
}
```

## Note on Current Implementation

The current `ShaderData.cpp` contains placeholder SPIR-V data. For a working triangle, you should:

1. Compile the shaders above using `glslc`
2. Replace the placeholder data in `ShaderData.cpp` with the actual compiled SPIR-V
3. Or implement runtime compilation using shaderc

The shader structure is correct - you just need valid SPIR-V bytecode!
