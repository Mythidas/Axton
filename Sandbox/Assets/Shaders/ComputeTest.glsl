#version 430 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout (rgba32f, binding = 0) uniform image2D imgOutput;
layout(std140, binding = 0) uniform Camera
{
	vec4 u_CamPosition;
	vec4 u_CamDirection;
	mat4 u_View;
	mat4 u_Projection;
};

void main()
{
	ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
	vec4 color = vec4(0, 0, 0, 1.0);
	vec3 sphCen = vec3(0.0, 0.0, -1.6);
	vec3 sphColor = vec3(1.0, 0.0, 1.0);
	float sphRad = 2.0;
	vec3 light = vec3(-1.0, -1.0, -1.0);

	// Ray
	float forwardX = (float(texelCoord.x) / gl_NumWorkGroups.x) * 2.0 - 1.0;
	float forwardY = (float(texelCoord.y) / gl_NumWorkGroups.y) * 2.0 - 1.0;
	vec4 target = u_Projection * vec4(forwardX, forwardY, 1.0, 1.0);
	vec3 rayOrg = u_CamPosition.xyz;
	vec3 rayDir = vec3(u_View * vec4(normalize(vec3(target) / target.w), 0));

	vec3 oc = rayOrg - sphCen;
	float a = dot(rayDir, rayDir);
	float b = 2.0f * dot(rayDir, oc);
	float c = dot(oc, oc) - sphRad * sphRad;
	
	float discrim = b * b - 4.0 * a * c;

	if (discrim >= 0.0)
	{
		float t0 = (-b - sqrt(discrim)) / (2.0 * a);
		if (t0 > 0.0)
		{
			vec3 p = rayOrg + rayDir * t0;
			vec3 n = normalize(p - sphCen);
			float d = max(dot(n, -light), 0.0);
			color = vec4(sphColor * d, 1.0);
		}
	}

	imageStore(imgOutput, texelCoord, color);
}