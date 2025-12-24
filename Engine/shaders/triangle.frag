#version 450

// Simple Triangle Fragment Shader

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

void main() {
    // Output color (red triangle)
    outColor = vec4(fragColor, 1.0);
}
