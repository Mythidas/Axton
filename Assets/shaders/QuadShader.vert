#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aTexIndex;

out vec4 VertexColor;
out	vec2 TexCoord;
out float TexIndex;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

void main()
{
	gl_Position = u_ViewProjection * vec4(aPos, 1.0);
	VertexColor = aColor;
	TexCoord = aTexCoord;
	TexIndex = aTexIndex;
}