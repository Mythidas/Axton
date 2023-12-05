#version 460 core

out vec4 FragColor;

in vec4 VertexColor;
in vec2 TexCoord;
in float TexIndex;

layout (binding = 0) uniform sampler2D u_Textures[32];

void main()
{
	vec4 texColor = VertexColor;

	switch(int(TexIndex + 0.01))
	{
		case 0: texColor *= texture(u_Textures[0], TexCoord); break;
		case 1: texColor *= texture(u_Textures[1], TexCoord); break;
		case 2: texColor *= texture(u_Textures[2], TexCoord); break;
	}

	FragColor = texColor;
}