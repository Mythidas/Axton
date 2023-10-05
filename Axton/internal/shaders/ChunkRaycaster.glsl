#version 430 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout (rgba32f, binding = 0) uniform image2D imgOutput;

layout (std140, binding = 0) uniform camera
{
	vec4 u_CamPosition;
	vec4 u_CamDirection;
	mat4 u_View;
	mat4 u_Projection;
};

struct Voxel
{
	uint Color;
};

layout (std430, binding = 2) readonly buffer voxelStorage
{
	Voxel[] u_Voxels;
};

const int CHUNK_SIZE = 4;
const float VOXEL_SIZE = 1;

struct Ray
{
	vec3 Origin;
	vec3 Direction;
};

vec3 getRayPoint(Ray ray, float t)
{
	return ray.Origin + ray.Direction * (t + 0.00001);
}

Voxel getVoxel(ivec3 index)
{
	return u_Voxels[index.x + CHUNK_SIZE * (index.y + CHUNK_SIZE * index.z)];
}

vec4 unpackColor(uint color)
{
	vec4 unpacked;
	unpacked.x = float((color >> 24) & 0xFF) / 255.0;
	unpacked.y = float((color >> 16) & 0xFF) / 255.0;
	unpacked.z = float((color >> 8) & 0xFF) / 255.0;
	unpacked.w = float((color) & 0xFF) / 255.0;
	return unpacked;
}

struct RayTracePayload
{
	vec3 Point;
	vec3 Normal;
	float Distance;
	int ObjIndex;
};

RayTracePayload missHit(Ray ray)
{
	RayTracePayload payload;
	payload.Distance = -1;
	return payload;
}

vec3 getVoxelNormal(vec3 center, vec3 hit)
{
	vec3 diff = abs(hit - center);
	if (diff.x > diff.y && diff.x > diff.z)
		return vec3(sign(diff.x), 0, 0);
	else if (diff.y > diff.z)
		return vec3 (0, sign(diff.y), 0);
	else
		return vec3(0, 0, sign(diff.z));
}

RayTracePayload closestHitBox(Ray ray, vec3 position, float hitDistance, int objIndex)
{
	RayTracePayload payload;
	payload.Distance = hitDistance;
	payload.ObjIndex = objIndex;
	payload.Point = getRayPoint(ray, hitDistance);
	payload.Normal = getVoxelNormal(position, payload.Point);
	return payload;
}

vec2 findBoxTminTmax(vec3 minExtent, vec3 maxExtent, Ray ray)
{
	float tmin = 0;
	float tmax = 5000;

	vec3 invDir = 1 / ray.Direction;

	vec3 t0s = (minExtent - ray.Origin) * invDir;
	vec3 t1s = (maxExtent - ray.Origin) * invDir;

	vec3 tsmaller = min(t0s, t1s);
	vec3 tbigger = max(t0s, t1s);

	tmin = max(tmin, max(tsmaller[0], max(tsmaller[1], tsmaller[2])));
	tmax = min(tmax, min(tbigger[0], min(tbigger[1], tbigger[2])));

	return vec2(tmin, tmax);
}

RayTracePayload traceBoxes(Ray ray)
{
	float closestPoint = 5000;
	int closestBox = -1;

	vec3 position = vec3(0, 0, -10);
	vec3 minExtent = position - VOXEL_SIZE * CHUNK_SIZE * 0.5;
	vec3 maxExtent = position + VOXEL_SIZE * CHUNK_SIZE * 0.5;
	vec2 minMax = findBoxTminTmax(minExtent, maxExtent, ray);
	float tmin = minMax.x;
	float tmax = minMax.y;

	if (tmin <= tmax)
	{
		// Find the Voxel where the ray starts
		vec3 startPos = getRayPoint(ray, tmin);
		ivec3 currentIndex = ivec3(floor((startPos - minExtent)));
		ivec3 steps = ivec3(sign(ray.Direction));

		vec3 tDelta = 1 / ray.Direction;
		vec3 tMax = vec3(tmax);
		for (int j = 0; j < 3; j++)
		{
			if (ray.Direction[j] > 0)
				tMax[j] = (minExtent[j] + currentIndex[j] + 1 - startPos[j]) / ray.Direction[j];
			else if (ray.Direction[j] < 0)
			{
				tMax[j] = (minExtent[j] + currentIndex[j] - startPos[j]) / ray.Direction[j];
				tDelta[j] = 1 / -ray.Direction[j];
			}
		}

		for (int j = 0; j < 100; j++)
		{
			if (currentIndex.x < 0 || currentIndex.x >= CHUNK_SIZE
				|| currentIndex.y < 0 || currentIndex.y >= CHUNK_SIZE
				|| currentIndex.z < 0 || currentIndex.z >= CHUNK_SIZE)
			{
				break;
			}

			float solid = unpackColor(getVoxel(currentIndex).Color).x;
			if (solid > 0.5)
			{
				vec2 tValue = findBoxTminTmax(minExtent + currentIndex, minExtent + currentIndex + VOXEL_SIZE, ray);
				position = (minExtent + currentIndex + VOXEL_SIZE * 0.5);
				closestPoint = tValue.x;
				closestBox = 1;
				break;
			}

			if (tMax.x < tMax.y && tMax.x < tMax.z)
			{
				tMax.x += tDelta.x;
				currentIndex.x += steps.x;
			}
			else if (tMax.y < tMax.z)
			{
				tMax.y += tDelta.y;
				currentIndex.y += steps.y;
			}
			else
			{
				tMax.z += tDelta.z;
				currentIndex.z += steps.z;
			}
		}
	}

	if (closestBox < 0) return missHit(ray);

	return closestHitBox(ray, position, closestPoint, closestBox);
}

void main()
{
	vec4 finalColor = vec4(0, 0, 0, 1);
	ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

	ivec2 screenSize = imageSize(imgOutput);
	float horizonCoefficient = (float(texelCoord.x) * 2 - screenSize.x) / screenSize.x;
	float verticalCoefficient = (float(texelCoord.y) * 2 - screenSize.y) / screenSize.y;
	vec4 pixelSample = vec4(horizonCoefficient, verticalCoefficient, 1.0, 1.0);

	Ray ray;
	ray.Origin = u_CamPosition.xyz;

	vec4 target = u_Projection * pixelSample;
	ray.Direction = normalize(vec3(u_View * vec4(normalize(vec3(target) / target.w), 0)));

	RayTracePayload payload = traceBoxes(ray);

	if (payload.Distance > 0)
	{
		finalColor = vec4(payload.Normal, 1);
	}

	imageStore(imgOutput, texelCoord, finalColor);
}