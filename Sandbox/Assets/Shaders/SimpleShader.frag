#version 330 core

out vec4 FragColor;

in vec4 outColor;
in vec2 outTexCoord;
in float outTexIndex;

void main()
{
	FragColor = outColor;
}