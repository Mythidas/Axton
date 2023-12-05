#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aTexIndex;

out vec4 VertexColor;
out	vec2 TexCoord;
out float TexIndex;

void main()
{
	gl_Position = vec4(aPos, 0.0, 1.0);
	VertexColor = aColor;
	TexCoord = aTexCoord;
	TexIndex = aTexIndex;
}