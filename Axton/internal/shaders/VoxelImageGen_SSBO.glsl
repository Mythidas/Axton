#version 430 core

layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
layout (rgba32f, binding = 0) writeonly uniform image2D imgOutput;

layout (std140, binding = 0) uniform camera
{
	vec4 u_CamPosition;
	vec4 u_CamDirection;
	mat4 u_View;
	mat4 u_Projection;
	uint u_RenderPass;
};

struct Material
{
	uint Albedo;
};

struct Chunk
{
	vec3 MinExtent;
	vec3 MaxExtent;
	ivec3 GridSize;
	float CamDistance;
	uint VoxelOffset;
	Material[255] Materials;
};

struct Voxel
{
	uint MatIndex;
};

layout (std430, binding = 2) readonly buffer chunkStorage
{
	Chunk[] u_Chunks;
};

layout (std430, binding = 4) readonly buffer voxStorage
{
	Voxel[] u_Voxels;
};

const float VOXEL_SIZE = 1;

uint collapseIndex(ivec3 index, Chunk chunk)
{
	return uint(((chunk.GridSize.x * chunk.GridSize.y * index.z) + (chunk.GridSize.x * index.y) + index.x));	
}

uint getVoxelMaterial(ivec3 index, Chunk chunk)
{
	uint cIndex = collapseIndex(index, chunk);
	uint buff = cIndex % 4;
	uint smallIndex = uint(floor(cIndex / 4)) + chunk.VoxelOffset;

	vec4 unpacked = unpackUnorm4x8(u_Voxels[smallIndex].MatIndex);
	switch (buff)
	{
		case 0: return uint(unpacked.x * 255.0); break;
		case 1: return uint(unpacked.y * 255.0); break;
		case 2: return uint(unpacked.z * 255.0); break;
		case 3: return uint(unpacked.w * 255.0); break;
	}

	return uint(unpacked.w * 255);
}

struct Ray
{
	vec3 Origin;
	vec3 Direction;
	vec3 InvDirection;
};

vec3 getRayPoint(Ray ray, double t)
{
	return vec3(ray.Origin + ray.Direction * (t + 0.00001));
}

struct RayTracePayload
{
	vec3 Point;
	vec3 Normal;
	float Distance;
	int ObjIndex;
	uint MaterialIndex;
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

RayTracePayload closestHitBox(Ray ray, vec3 position, float hitDistance, int objIndex, uint material)
{
	RayTracePayload payload;
	payload.Distance = hitDistance;
	payload.ObjIndex = objIndex;
	payload.MaterialIndex = material;
	payload.Point = getRayPoint(ray, hitDistance);
	payload.Normal = getVoxelNormal(position, payload.Point);
	return payload;
}

vec2 findBoxTminTmax(vec3 minExtent, vec3 maxExtent, Ray ray)
{
	float tmin = 0;
	float tmax = 100000000;

	vec3 t0s = (minExtent - ray.Origin) * ray.InvDirection;
	vec3 t1s = (maxExtent - ray.Origin) * ray.InvDirection;

	vec3 tsmaller = min(t0s, t1s);
	vec3 tbigger = max(t0s, t1s);

	tmin = max(tmin, max(tsmaller[0], max(tsmaller[1], tsmaller[2])));
	tmax = min(tmax, min(tbigger[0], min(tbigger[1], tbigger[2])));

	return vec2(tmin, tmax);
}

bool findClosestVoxel(Ray ray, Chunk chunk, float tmin, float tmax, inout float closestPoint, inout vec3 closestPosition, inout uint closestMaterial)
{
	// Find the Voxel where the ray starts
	vec3 startPos = getRayPoint(ray, tmin + 0.003);
	ivec3 currentIndex = ivec3(floor((startPos - chunk.MinExtent) / VOXEL_SIZE));
	ivec3 steps = ivec3(sign(ray.Direction));

	vec3 tDelta = VOXEL_SIZE / ray.Direction;
	vec3 tMax = vec3(tmax);
	for (int j = 0; j < 3; j++)
	{
		if (ray.Direction[j] > 0)
			tMax[j] = tmin + (chunk.MinExtent[j] + (currentIndex[j] + 1) * VOXEL_SIZE - startPos[j]) / ray.Direction[j];
		else if (ray.Direction[j] < 0)
		{
			tMax[j] = tmin + (chunk.MinExtent[j] + currentIndex[j] * VOXEL_SIZE - startPos[j]) / ray.Direction[j];
			tDelta[j] = VOXEL_SIZE / -ray.Direction[j];
		}
	}

	for (int j = 0; j < chunk.GridSize.x * chunk.GridSize.y * chunk.GridSize.z; j++)
	{
		if (currentIndex.x < 0 || currentIndex.x >= chunk.GridSize.x
			|| currentIndex.y < 0 || currentIndex.y >= chunk.GridSize.y
			|| currentIndex.z < 0 || currentIndex.z >= chunk.GridSize.z)
		{
			break;
		}

		uint matIndex = getVoxelMaterial(currentIndex, chunk);
		if (matIndex != 0)
		{
			vec2 tValue = findBoxTminTmax(chunk.MinExtent + currentIndex * VOXEL_SIZE, 
							chunk.MinExtent + (currentIndex + 1) * VOXEL_SIZE, ray);

			closestPosition = chunk.MinExtent + vec3(currentIndex * VOXEL_SIZE + VOXEL_SIZE * 0.5);
			closestPoint = tValue.x;
			closestMaterial = matIndex;
			return true;
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
	
	return false;
}

RayTracePayload traceBoxes(Ray ray)
{
	float closestPoint = 100000000;
	int closestChunk = -1;
	vec3 closestPosition = vec3(0);
	uint closestMaterial = -1;

	for (int i = 0; i < u_Chunks.length(); i++)
	{
		vec2 minMax = findBoxTminTmax(u_Chunks[i].MinExtent, u_Chunks[i].MaxExtent, ray);
		float tmin = minMax.x;
		float tmax = minMax.y;

		if (tmin <= tmax && tmin < closestPoint)
		{
			if (findClosestVoxel(ray, u_Chunks[i], tmin, tmax, closestPoint, closestPosition, closestMaterial))
			{
				closestChunk = i;
				break;
			}
		}
	}

	if (closestChunk < 0) return missHit(ray);

	return closestHitBox(ray, closestPosition, closestPoint, closestChunk, closestMaterial);
}

void main()
{
	vec4 finalColor = vec4(0, 0, 0, 1);
	ivec2 screenSize = imageSize(imgOutput);
	ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

	float horizonCoefficient = (float(texelCoord.x) * 2 - screenSize.x) / screenSize.x;
	float verticalCoefficient = (float(texelCoord.y) * 2 - screenSize.y) / screenSize.y;
	vec4 pixelSample = vec4(horizonCoefficient, verticalCoefficient, 1.0, 1.0);

	Ray ray;
	ray.Origin = u_CamPosition.xyz;

	vec4 target = u_Projection * pixelSample;
	ray.Direction = normalize(vec3(u_View * vec4(normalize(vec3(target) / target.w), 0)));
	ray.InvDirection = 1 / ray.Direction;

	RayTracePayload payload = traceBoxes(ray);

	if (payload.Distance > 0)
	{
		switch (u_RenderPass)
		{
		case 0: // Full Render Pass
			{
				vec4 color = unpackUnorm4x8(u_Chunks[payload.ObjIndex].Materials[payload.MaterialIndex].Albedo);
				finalColor = color;
				imageStore(imgOutput, texelCoord, finalColor);
				break;
			}
		case 1: // Render Normals
			{
				vec4 color = vec4(payload.Normal, 1);
				finalColor = color;
				imageStore(imgOutput, texelCoord, finalColor);
				break;
			}
		case 2: // Render Albedos
			{
				vec4 color = unpackUnorm4x8(u_Chunks[payload.ObjIndex].Materials[payload.MaterialIndex].Albedo);
				finalColor = color;
				imageStore(imgOutput, texelCoord, finalColor);
				break;
			}
		}
	}
	else
	{
		imageStore(imgOutput, texelCoord, finalColor);
	}
}