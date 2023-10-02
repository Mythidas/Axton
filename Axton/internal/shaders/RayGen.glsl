#version 430 core

layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
layout (rgba32f, binding = 0) uniform image2D imgOutput;

// Object Structs
struct Sphere
{
	vec3 Position;
	float Radius;
	int MatIndex;
};

struct Box
{
	vec3 Position;
	vec3 Extents;
	int MatIndex;
};

struct Light
{
	vec3 Position;
	vec3 Direction;
	vec4 Color;
};

struct Material
{
	vec4 Albedo;
	float Roughness;
};

struct RayTracePayload
{
	vec3 Point;
	vec3 Normal;
	float Distance;
	int ObjIndex;
};

struct Ray
{
	vec3 Origin;
	vec3 Direction;
};

// Data Buffers
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
layout (std430, binding = 3) readonly buffer boxStorage
{
	Box[] u_Boxes;
};
layout (std430, binding = 4) readonly buffer lightStorage
{
	Light[] u_Lights;
};
layout (std430, binding = 5) readonly buffer materialStorage
{
	Material[] u_Materials;
};

// Helper functions

// Random Number Generator //
uint seed = 0u;
void hashSeed()
{
    seed ^= 2747636419u;
    seed *= 2654435769u;
    seed ^= seed >> 16;
    seed *= 2654435769u;
    seed ^= seed >> 16;
    seed *= 2654435769u;
}
void initRandomGenerator(vec2 fragCoord)
{
    seed = uint(fragCoord.y* imageSize(imgOutput).x + fragCoord.x)+uint(gl_LocalInvocationIndex)*uint(imageSize(imgOutput).x)*uint(imageSize(imgOutput).y);
}
float random()
{
    hashSeed();
    return (float(seed)/4294967295.0);
}
float random(float fMin, float fMax)
{
    hashSeed();
    return fMin + (fMax - fMin) * (float(seed)/4294967295.0);
}
vec3 randVec3()
{
	return vec3(random(), random(), random());
}
vec3 randVec3(float fMin, float fMax)
{
	return vec3(random(fMin, fMax), random(fMin, fMax), random(fMin, fMax));
}
vec3 randVec3Unit()
{
	while (true)
	{	
		vec3 p = randVec3(-1, 1);
		if (sqrt(length(p)) < 1) return p;
	}
}
vec3 randomUnitVector()
{
	return normalize(randVec3Unit());
}
vec3 randomOnHemisphere(const vec3 normal)
{
	vec3 onSphere = randomUnitVector();
	if (dot(onSphere, normal) > 0) return onSphere;

	return -onSphere;
}


// Called when closest ray hit is found
RayTracePayload closestHitSphere(Ray ray, float hitDistance, int objIndex)
{
	RayTracePayload payload;
	payload.Distance = hitDistance;
	payload.ObjIndex = objIndex;
	payload.Point = ray.Origin + ray.Direction * hitDistance;
	payload.Normal = normalize(payload.Point - u_Spheres[objIndex].Position.xyz);
	return payload;
}

RayTracePayload closestHitBox(Ray ray, float hitDistance, int objIndex)
{
	RayTracePayload payload;
	payload.Distance = hitDistance;
	payload.ObjIndex = objIndex;
	payload.Point = ray.Origin + ray.Direction * (hitDistance - 0.00001);

	vec3 minExtent = u_Boxes[objIndex].Position - u_Boxes[objIndex].Extents;
	vec3 maxExtent = u_Boxes[objIndex].Position + u_Boxes[objIndex].Extents;
	vec3 p = payload.Point - u_Boxes[objIndex].Position;
	float dx = abs((minExtent.x - maxExtent.x) / 2);
	float dy = abs((minExtent.y - maxExtent.y) / 2);
	float dz = abs((minExtent.z - maxExtent.z) / 2);
	payload.Normal = normalize(vec3(int(p.x / dx), int(p.y / dy), int(p.z / dz)));
	return payload;
}

// Called when no ray hit is found
RayTracePayload missHit(Ray ray)
{
	RayTracePayload payload;
	payload.Distance = -1;
	return payload;
}

// Ray trace for sphere intersections
RayTracePayload traceRaySpheres(Ray ray)
{
	float closestPoint = 5000;
	int closestSphereIndex = -1;
	for (int i = 0; i < u_Spheres.length(); i++)
	{
		vec3 oc = ray.Origin - u_Spheres[i].Position.xyz;
		float a = dot(ray.Direction, ray.Direction);
		float b = 2.0f * dot(ray.Direction, oc);
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

	if (closestSphereIndex < 0)
	{
		return missHit(ray);
	}

	return closestHitSphere(ray, closestPoint, closestSphereIndex);
}

// Ray trace for box intersections
RayTracePayload traceRayBoxes(Ray ray)
{
	float closestPoint = 5000;
	int closestBox = -1;
	for (int i = 0; i < u_Boxes.length(); i++)
	{
		vec3 minExtent = u_Boxes[i].Position - u_Boxes[i].Extents;
		vec3 maxExtent = u_Boxes[i].Position + u_Boxes[i].Extents;

		float tmin = 0;
		float tmax = 5000;

		vec3 invDir = 1 / ray.Direction;

		vec3 t0s = (minExtent - ray.Origin) * invDir;
		vec3 t1s = (maxExtent - ray.Origin) * invDir;

		vec3 tsmaller = min(t0s, t1s);
		vec3 tbigger = max(t0s, t1s);

		tmin = max(tmin, max(tsmaller[0], max(tsmaller[1], tsmaller[2])));
		tmax = min(tmax, min(tbigger[0], min(tbigger[1], tbigger[2])));

		if (tmin < tmax && tmin < closestPoint)
		{
			closestPoint = tmin;
			closestBox = i;
		}
	}

	if (closestBox < 0) return missHit(ray);

	return closestHitBox(ray, closestPoint, closestBox);
}

Ray getRay()
{
	// Calculating the current x and y of the pixels
	float px = -0.5 + random(0, 1);
	float py = -0.5 + random(0, 1);
	ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
	ivec2 screenSize = imageSize(imgOutput);
	float horizonCoefficient = (float(texelCoord.x + px) * 2 - screenSize.x) / screenSize.x;
	float verticalCoefficient = (float(texelCoord.y + py) * 2 - screenSize.y) / screenSize.y;
	vec4 pixelSample = vec4(horizonCoefficient, verticalCoefficient, 1.0, 1.0);

	// Ray
	Ray ray;
	ray.Origin = u_CamPosition.xyz;

	vec4 target = u_Projection * pixelSample;
	ray.Direction = vec3(u_View * vec4(normalize(vec3(target) / target.w), 0));

	return ray;
}

void main()
{
	initRandomGenerator(gl_GlobalInvocationID.xy);
	vec4 finalColor = vec4(0, 0, 0, 1.0);
	float samples = 15;

	ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
	ivec2 screenSize = imageSize(imgOutput);

//			RayTracePayload payload = traceRayBoxes(ray);
//
//		if (payload.Distance > 0)
//		{
//			float intensity = max(0, dot(payload.Normal, -normalize(u_Lights[0].Direction)));
//			finalColor = u_Materials[u_Boxes[payload.ObjIndex].MatIndex].Albedo * (intensity + 0.3);
//		}

	for (int s = 0; s < samples; s++)
	{
		Ray ray = getRay();
		
		float strength = 1;
		for (int i = 0; i < 2; i++)
		{
			RayTracePayload payload = traceRayBoxes(ray);

			if (payload.Distance > 0)
			{
				float intensity = max(0, dot(payload.Normal, -normalize(u_Lights[0].Direction)));
				vec3 dir = randomOnHemisphere(payload.Normal);
				ray.Origin = payload.Point;
				ray.Direction = dir;
				strength *= 0.5;
			}
			else break;
		}

		vec3 unitDir = normalize(ray.Direction);
		float a = 0.5 * (unitDir.y + 1);
		finalColor += vec4((1 - a) * vec3(1) + a * vec3(0.5, 0.7, 1.0), 1.0) * strength;
	}

	finalColor /= samples;

	imageStore(imgOutput, texelCoord, finalColor);
}