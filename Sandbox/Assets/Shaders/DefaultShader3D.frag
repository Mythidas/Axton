#version 460

out vec4 FragColor;

in vec4 VertexColor;
in vec3 TexCoord;
in float TexIndex;

uniform sampler3D u_Textures[32];

void main()
{
	int index = int(TexIndex);
	FragColor = VertexColor * texture(u_Textures[index], TexCoord);
}