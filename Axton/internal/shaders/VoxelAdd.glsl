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

struct Chunk
{
	vec3 MinExtent;
	vec3 MaxExtent;
	ivec3 GridSize;
	uint VoxelOffset;
	uint[64] MaterialLookup;
};

struct Voxel
{
	uint MatIndex;
};

layout (std430, binding = 2) readonly buffer chunkStorage
{
	Chunk[] u_Chunks;
};

layout (std430, binding = 4) buffer voxStorage
{
	Voxel[] u_Voxels;
};

const float VOXEL_SIZE = 1;

uint collapseIndex(ivec3 index, Chunk chunk)
{
	return uint(((chunk.GridSize.x * chunk.GridSize.y * index.z) + (chunk.GridSize.x * index.y) + index.x) + chunk.VoxelOffset);	
}

uint getVoxelMaterial(ivec3 index, Chunk chunk)
{
	uint cIndex = collapseIndex(index, chunk);
	uint sIndex = cIndex / 4;
	uint bIndex = cIndex % 4;

	vec4 mats = unpackUnorm4x8(u_Voxels[sIndex].MatIndex);
	switch (bIndex)
	{
		case 0: return uint(mats.x * 255);
		case 1: return uint(mats.y * 255);
		case 2: return uint(mats.z * 255);
		case 3: return uint(mats.w * 255);
	}

	return 0;
}

void setVoxelMaterial(uint value, ivec3 index, Chunk chunk)
{
	ivec3 clamped = clamp(index, ivec3(0), chunk.GridSize);
	uint cIndex = collapseIndex(index, chunk);
	uint sIndex = cIndex / 4;
	uint bIndex = cIndex % 4;

	switch (bIndex)
	{
		case 0: u_Voxels[sIndex].MatIndex |= (value << 24); break;
		case 1: u_Voxels[sIndex].MatIndex |= (value << 16); break;
		case 2: u_Voxels[sIndex].MatIndex |= (value << 8); break;
		case 3: u_Voxels[sIndex].MatIndex |= (value << 0); break;
	}
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

void traceBoxes(Ray ray)
{
	float closestPoint = 100000000;
	int closestVoxel = -1;
	vec3 closestPosition = vec3(0);
	ivec3 closestIndex = ivec3(0);

	for (int i = 0; i < u_Chunks.length(); i++)
	{
		vec2 minMax = findBoxTminTmax(u_Chunks[i].MinExtent, u_Chunks[i].MaxExtent, ray);
		float tmin = minMax.x;
		float tmax = minMax.y;

		if (tmin <= tmax && tmin < closestPoint)
		{
			// Find the Voxel where the ray starts
			vec3 startPos = getRayPoint(ray, tmin + 0.003);
			ivec3 currentIndex = ivec3(floor((startPos - u_Chunks[i].MinExtent) / VOXEL_SIZE));
			ivec3 steps = ivec3(sign(ray.Direction));

			vec3 tDelta = VOXEL_SIZE / ray.Direction;
			vec3 tMax = vec3(tmax);
			for (int j = 0; j < 3; j++)
			{
				if (ray.Direction[j] > 0)
					tMax[j] = tmin + (u_Chunks[i].MinExtent[j] + (currentIndex[j] + 1) * VOXEL_SIZE - startPos[j]) / ray.Direction[j];
				else if (ray.Direction[j] < 0)
				{
					tMax[j] = tmin + (u_Chunks[i].MinExtent[j] + currentIndex[j] * VOXEL_SIZE - startPos[j]) / ray.Direction[j];
					tDelta[j] = VOXEL_SIZE / -ray.Direction[j];
				}
			}

			for (int j = 0; j < u_Chunks[i].GridSize.x * u_Chunks[i].GridSize.y * u_Chunks[i].GridSize.z; j++)
			{
				if (currentIndex.x < 0 || currentIndex.x >= u_Chunks[i].GridSize.x
					|| currentIndex.y < 0 || currentIndex.y >= u_Chunks[i].GridSize.y
					|| currentIndex.z < 0 || currentIndex.z >= u_Chunks[i].GridSize.z)
				{
					break;
				}

				uint matIndex = getVoxelMaterial(currentIndex, u_Chunks[i]);
				if (matIndex != 0)
				{
					vec2 tValue = findBoxTminTmax(u_Chunks[i].MinExtent + currentIndex * VOXEL_SIZE, 
									u_Chunks[i].MinExtent + (currentIndex + 1) * VOXEL_SIZE, ray);

					closestPosition = u_Chunks[i].MinExtent + vec3(currentIndex * VOXEL_SIZE + VOXEL_SIZE * 0.5);
					closestPoint = tValue.x;
					closestVoxel = i;
					closestIndex = currentIndex;
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
	}

	if (closestVoxel >= 0)
	{
		ivec3 index1 = ivec3(closestIndex);
		ivec3 index2 = ivec3(closestIndex);
		ivec3 index3 = ivec3(closestIndex);
		ivec3 index4 = ivec3(closestIndex);
		ivec3 index5 = ivec3(closestIndex);
		ivec3 index6 = ivec3(closestIndex);

		index1.x += 1;
		index2.x -= 1;
		index3.y += 1;
		index4.y -= 1;
		index5.z += 1;
		index6.z -= 1;

		setVoxelMaterial(1, closestIndex, u_Chunks[closestVoxel]);
		setVoxelMaterial(1, index1, u_Chunks[closestVoxel]);
		setVoxelMaterial(1, index2, u_Chunks[closestVoxel]);
		setVoxelMaterial(1, index3, u_Chunks[closestVoxel]);
		setVoxelMaterial(1, index4, u_Chunks[closestVoxel]);
		setVoxelMaterial(1, index5, u_Chunks[closestVoxel]);
		setVoxelMaterial(1, index6, u_Chunks[closestVoxel]);
	}
}

void main()
{
	vec4 finalColor = vec4(0, 0, 0, 1);
	ivec2 screenSize = imageSize(imgOutput) / 2;
	ivec2 texelCoord = ivec2(screenSize / 2);

	float horizonCoefficient = (float(texelCoord.x) * 2 - screenSize.x) / screenSize.x;
	float verticalCoefficient = (float(texelCoord.y) * 2 - screenSize.y) / screenSize.y;
	vec4 pixelSample = vec4(horizonCoefficient, verticalCoefficient, 1.0, 1.0);

	Ray ray;
	ray.Origin = u_CamPosition.xyz;

	vec4 target = u_Projection * pixelSample;
	ray.Direction = normalize(vec3(u_View * vec4(normalize(vec3(target) / target.w), 0)));
	ray.InvDirection = 1 / ray.Direction;

	traceBoxes(ray);
}