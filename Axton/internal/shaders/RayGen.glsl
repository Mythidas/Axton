#version 430 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout (rgba32f, binding = 0) uniform image2D imgOutput;

struct Sphere
{
	vec4 Position;
	float Radius;
	int MatIndex;
};

struct Material
{
	vec4 Albedo;
	float Roughness;
};

layout(std140, binding = 0) uniform Camera
{
	vec4 u_CamPosition;
	vec4 u_CamDirection;
	mat4 u_View;
	mat4 u_Projection;
};
layout (std430, binding = 2) readonly buffer sphereStorage
{
	Sphere[] u_Spheres;
};
layout (std430, binding = 3) readonly buffer materialStorage
{
	Material[] u_Materials;
};

void main()
{
	ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
	vec4 color = vec4(0, 0, 0, 1.0);
	vec3 light = vec3(-1.0, -1.0, -1.0);

	// Ray
	float forwardX = (float(texelCoord.x) / gl_NumWorkGroups.x) * 2.0 - 1.0;
	float forwardY = (float(texelCoord.y) / gl_NumWorkGroups.y) * 2.0 - 1.0;
	vec4 target = u_Projection * vec4(forwardX, forwardY, 1.0, 1.0);
	vec3 rayOrg = u_CamPosition.xyz;
	vec3 rayDir = vec3(u_View * vec4(normalize(vec3(target) / target.w), 0));

	float closestPoint = 5000;
	int closestSphereIndex = -1;
	for (int i = 0; i < u_Spheres.length(); i++)
	{
		vec3 oc = rayOrg - u_Spheres[i].Position.xyz;
		float a = dot(rayDir, rayDir);
		float b = 2.0f * dot(rayDir, oc);
		float c = dot(oc, oc) - u_Spheres[i].Radius * u_Spheres[i].Radius;
	
		float discrim = b * b - 4.0 * a * c;

		if (discrim >= 0.0)
		{
			float t0 = (-b - sqrt(discrim)) / (2.0 * a);
			if (t0 > 0 && t0 < closestPoint)
			{
				closestPoint = t0;
				closestSphereIndex = i;
			}
		}
	}

	if (closestSphereIndex > -1)
	{
		vec3 p = rayOrg + rayDir * closestPoint;
		vec3 n = normalize(p - u_Spheres[closestSphereIndex].Position.xyz);
		float d = max(dot(n, -light), 0.0);
		color = vec4(u_Materials[u_Spheres[closestSphereIndex].MatIndex].Albedo.xyz * d, 1.0);	
	}

	imageStore(imgOutput, texelCoord, color);
}