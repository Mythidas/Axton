#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aTexIndex;

out vec4 outColor;
out vec2 outTexCoord;
out float outTexIndex;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	outColor = aColor;
	outTexCoord = aTexCoord;
	outTexIndex = aTexIndex;
}