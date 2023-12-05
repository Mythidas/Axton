#version 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec3 aTexCoord;
layout (location = 3) in float aTexIndex;

out vec4 VertexColor;
out	vec3 TexCoord;
out float TexIndex;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
	VertexColor = aColor;
	TexCoord = aTexCoord;
	TexIndex = aTexIndex;
}