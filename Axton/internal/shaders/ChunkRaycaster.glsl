#version 430 core

layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
layout (rgba32f, binding = 0) uniform image2D imgOutput;
layout (r8, binding = 1) uniform image3D voxelData[7];

layout (std140, binding = 0) uniform camera
{
	vec4 u_CamPosition;
	vec4 u_CamDirection;
	mat4 u_View;
	mat4 u_Projection;
};

struct Material
{
	vec4 Albedo;
};

struct Chunk
{
	vec3 MinExtent;
	vec3 MaxExtent;
	ivec3 VoxelOffset;
	uint VoxelIndex;
	uint[64] MaterialLookup;
};

layout (std430, binding = 2) readonly buffer chunkStorage
{
	Chunk[] u_Chunks;
};

layout (std430, binding = 3) readonly buffer matStorage
{
	Material[] u_Materials;
};

const float VOXEL_SIZE = 1;

int getVoxelData(uint voxel, ivec3 index)
{
	return int(imageLoad(voxelData[voxel], ivec3(index)).r * 255);
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
	int MaterialIndex;
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

RayTracePayload closestHitBox(Ray ray, vec3 position, float hitDistance, int objIndex, int material)
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

RayTracePayload traceBoxes(Ray ray)
{
	float closestPoint = 100000000;
	int closestVoxel = -1;
	vec3 closestPosition = vec3(0);
	int closestMaterial = -1;

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

			vec3 gridSize = imageSize(voxelData[u_Chunks[i].VoxelIndex]);
			for (int j = 0; j < 100; j++)
			{
				if (currentIndex.x < 0 || currentIndex.x >= gridSize.x
					|| currentIndex.y < 0 || currentIndex.y >= gridSize.y
					|| currentIndex.z < 0 || currentIndex.z >= gridSize.z)
				{
					break;
				}

				int matIndex = getVoxelData(u_Chunks[i].VoxelIndex, currentIndex);
				if (matIndex != 255)
				{
					vec2 tValue = findBoxTminTmax(u_Chunks[i].MinExtent + currentIndex * VOXEL_SIZE, 
									u_Chunks[i].MinExtent + (currentIndex + 1) * VOXEL_SIZE, ray);

					closestPosition = u_Chunks[i].MinExtent + vec3(currentIndex * VOXEL_SIZE + VOXEL_SIZE * 0.5);
					closestPoint = tValue.x;
					closestVoxel = i;
					closestMaterial = matIndex;
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

	if (closestVoxel < 0) return missHit(ray);

	return closestHitBox(ray, closestPosition, closestPoint, closestVoxel, closestMaterial);
}

void main()
{
	vec4 finalColor = vec4(0, 0, 0, 1);
	ivec2 screenSize = imageSize(imgOutput) / 2;
	ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
	ivec2 texel2Coord = ivec2(texelCoord + screenSize);

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
		vec4 color = u_Materials[u_Chunks[payload.ObjIndex].MaterialLookup[payload.MaterialIndex]].Albedo;
		finalColor = color;
		imageStore(imgOutput, texelCoord, finalColor);

		vec4 normalColor = vec4(payload.Normal, 1);
		imageStore(imgOutput, texel2Coord, normalColor);
	}
	else
	{
		imageStore(imgOutput, texelCoord, finalColor);
		imageStore(imgOutput, texel2Coord, finalColor);
	}
}