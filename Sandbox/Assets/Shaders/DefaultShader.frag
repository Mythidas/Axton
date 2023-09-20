#version 330 core

out vec4 FragColor;

in vec4 VertexColor;
in vec2 TexCoord;
in float TexIndex;

uniform sampler2D u_Textures[32];

void main()
{
	int index = int(TexIndex);
	FragColor = VertexColor * texture(u_Textures[index], TexCoord);
}