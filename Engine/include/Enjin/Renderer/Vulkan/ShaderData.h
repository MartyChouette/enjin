#pragma once

#include "Enjin/Platform/Platform.h"
#include "Enjin/Platform/Types.h"
#include <vector>

namespace Enjin {
namespace Renderer {

// Pre-compiled SPIR-V shader data
// These are simple triangle shaders compiled to SPIR-V
namespace ShaderData {

// Simple vertex shader SPIR-V (compiled from GLSL)
// #version 450
// layout(location = 0) in vec3 inPosition;
// layout(location = 1) in vec3 inNormal;
// layout(location = 2) in vec2 inUV;
// layout(binding = 0) uniform UniformBufferObject {
//     mat4 model;
//     mat4 view;
//     mat4 proj;
// } ubo;
// layout(location = 0) out vec3 fragColor;
// void main() {
//     gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
//     fragColor = vec3(1.0, 0.0, 0.0); // Red color
// }
extern const std::vector<u32> TriangleVertexShader;

// Simple fragment shader SPIR-V
// #version 450
// layout(location = 0) in vec3 fragColor;
// layout(location = 0) out vec4 outColor;
// void main() {
//     outColor = vec4(fragColor, 1.0);
// }
extern const std::vector<u32> TriangleFragmentShader;

} // namespace ShaderData

} // namespace Renderer
} // namespace Enjin
