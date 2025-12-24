#version 450

// Simple Triangle Vertex Shader
// Supports position, normal, and UV attributes

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragUV;

void main() {
    // Transform vertex position
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    
    // Pass through color (red for now)
    fragColor = vec3(1.0, 0.0, 0.0);
    
    // Pass through UV
    fragUV = inUV;
}
