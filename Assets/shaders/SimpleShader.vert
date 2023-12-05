#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

vec4 colors[3] = vec4[](
    vec4(1.0, 0.0, 0.0, 1.0),
    vec4(0.0, 1.0, 0.0, 1.0),
    vec4(0.0, 0.0, 1.0, 1.0)
);

layout (location = 0) out vec4 outColor;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    outColor = aColor;
}