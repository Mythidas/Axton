#version 430 core

layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
layout (rgba32f, binding = 0) writeonly uniform image2D imgOutput;

layout (std140, binding = 0) uniform camera
{
	vec4 u_CamPosition;
	vec4 u_BackgroundColor;
	mat4 u_View;
	mat4 u_Projection;
	float u_RandomSeed;
	uint u_SampleSettings;
	uint u_RenderPass;
	uint u_Algorithm;
};

struct Chunk
{
	vec3 MinExtent;
	vec3 MaxExtent;
	ivec3 GridSize;
	bool Sparse;
	float VoxelSize;
	float CamDistance;
	uint VoxelOffset;
	uint MaterialIndex;
	uint[255] Albedos;
};

struct Voxel
{
	uint Material;
};

struct Material
{
	float Specular;
	float Metallic;
	float Roughness;
	float Emissive;
};

layout (std430, binding = 2) readonly buffer chunkStorage
{
	Chunk[] u_Chunks;
};

layout (std430, binding = 3) readonly buffer voxStorage
{
	Voxel[] u_Voxels;
};

layout (std430, binding = 4) readonly buffer materialStorage
{
	Material[] u_Materials;
};

int MARCHES = 0;

const float PI = 3.14159265359;
const float PHI = 1.61803398874989484820459; // Golden Ratio 
float EPSILON = 0.0000001; // Can be used for rounding errors

// Slow noise but very random
float gold_noise(in vec2 xy, in float seed)
{
	return fract(tan(distance(xy*PHI, xy)*seed)*xy.x);
}

// Faster but slightly less random, still preferred method
float custom_noise(vec2 st, float seed)
{
	return fract(sin(dot(st*PHI, vec2(12.9898, 78.233)) * seed) * 43758.5453123);
}

float lerp(float a, float b, float value)
{
	return a * (1.0 - value) + (b * value);
}

float noise(float seed)
{
	return lerp(0, 1, custom_noise(gl_GlobalInvocationID.xy, seed));
}

float noise(float seed, float low, float high)
{
	return lerp(low, high, custom_noise(gl_GlobalInvocationID.xy, seed));
}

vec3 randV3S(float low, float high)
{
	return vec3(noise(sin(u_RandomSeed + PHI), low, high), 
				noise(cos(u_RandomSeed), low, high), 
				noise(sin(u_RandomSeed * PHI), low, high));
}

vec3 randInUnitSphere()
{
	return normalize(randV3S(-1, 1));
}

uint collapseIndex(ivec3 index, Chunk chunk)
{
	return uint(((chunk.GridSize.x * chunk.GridSize.y * index.z) + (chunk.GridSize.x * index.y) + index.x));	
}

struct Ray
{
	vec3 Origin;
	vec3 Direction;
	vec3 InvDirection;
	vec3 Color;
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
	uint AlbedoIndex;
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
		return vec3(sign(hit.x - center.x), 0, 0);
	else if (diff.y > diff.z)
		return vec3 (0, sign(hit.y - center.y), 0);
	else
		return vec3(0, 0, sign(hit.z - center.z));
}

RayTracePayload closestHitBox(Ray ray, vec3 position, float hitDistance, int objIndex, uint albedo)
{
	RayTracePayload payload;
	payload.Distance = hitDistance;
	payload.ObjIndex = objIndex;
	payload.AlbedoIndex = albedo;
	payload.Point = getRayPoint(ray, hitDistance - EPSILON);
	payload.Normal = getVoxelNormal(position, payload.Point);
	return payload;
}

struct TValue
{
	float TMin; // Enter
	float TMax; // Exit
	vec3 T0; // Min
	vec3 T1; // Max
};

TValue findBoxTminTmax(vec3 minExtent, vec3 maxExtent, Ray ray)
{
	TValue value;
	value.TMin = 0;
	value.TMax = 10000000;

	vec3 t0s = (minExtent - ray.Origin) * ray.InvDirection;
	vec3 t1s = (maxExtent - ray.Origin) * ray.InvDirection;

	value.T0 = min(t0s, t1s);
	value.T1 = max(t0s, t1s);

	value.TMin = max(value.TMin, max(value.T0[0], max(value.T0[1], value.T0[2])));
	value.TMax = min(value.TMax, min(value.T1[0], min(value.T1[1], value.T1[2])));

	return value;
}

uint getVoxelInfoSparse(ivec3 index, Chunk chunk)
{
	int currentOffset = int(chunk.VoxelOffset);
	bvec3 mask;
	int levels = int(log2(chunk.GridSize.x));
	int childIndex = 0;
	int halfSize = 1 << (levels);

	for (int i = 0; i < levels; i++)
	{
		childIndex = 0;
		halfSize = halfSize >> 1;

		if (index.x >= halfSize)
		{
			childIndex |= 4;
			index.x -= halfSize;
		}
		if (index.y >= halfSize)
		{
			childIndex |= 2;
			index.y -= halfSize;
		}
		if (index.z >= halfSize)
		{
			childIndex |= 1;
			index.z -= halfSize;
		}

		ivec4 info = ivec4(unpackUnorm4x8(u_Voxels[currentOffset + childIndex].Material) * 255);
		if (info.w == 1)
		{
			return uint(info.x);
		}
		else
		{
			currentOffset += int(info.x + (info.y * 255) + (info.z * 255 * 255));
		}
	}
	
	return ivec4(unpackUnorm4x8(u_Voxels[currentOffset].Material) * 255).w;
}

uint getVoxelInfoDense(ivec3 index, Chunk chunk)
{
	uint pIndex = collapseIndex(index, chunk);
	uint bit = pIndex % 4;
	uint smallIndex = uint(floor(pIndex / 4)) + chunk.VoxelOffset;

	return uint(bitfieldExtract(u_Voxels[smallIndex].Material, int(bit * 8), 8));
}

uint getVoxelInfo(ivec3 index, Chunk chunk)
{
	if (chunk.Sparse)
		return getVoxelInfoSparse(index, chunk);
	else
		return getVoxelInfoDense(index, chunk);
}

// Amanatides & Woo 3DDA Traversal
bool awDDATraversal(Ray ray, Chunk chunk, TValue minMax, inout float closestPoint, inout vec3 closestPosition, inout uint closestMaterial)
{
	/// INITIALIZATION ///
	// Find the Voxel where the ray starts
	vec3 startPos = getRayPoint(ray, minMax.TMin + EPSILON * chunk.CamDistance * 0.05);
	ivec3 currentIndex = ivec3(floor((startPos - chunk.MinExtent) / chunk.VoxelSize));

	// Calculate XYZ step directions and distances
	ivec3 steps = ivec3(sign(ray.Direction) * chunk.VoxelSize);
	vec3 tDelta = steps * ray.InvDirection;
	vec3 tMax = vec3(minMax.TMax);
	float vTMin = minMax.TMin;
	for (int j = 0; j < 3; j++)
	{
		if (ray.Direction[j] > 0)
			tMax[j] = minMax.TMin + (chunk.MinExtent[j] + (currentIndex[j] + 1) * chunk.VoxelSize - startPos[j]) * ray.InvDirection[j];
		else if (ray.Direction[j] < 0)
		{
			tMax[j] = minMax.TMin + (chunk.MinExtent[j] + currentIndex[j] * chunk.VoxelSize - startPos[j]) * ray.InvDirection[j];
			tDelta[j] = chunk.VoxelSize * -ray.InvDirection[j];
		}
	}

	/// TRAVERSAL ///
	float epsAdjust = EPSILON * chunk.CamDistance * 0.06; // Adjusting the point to offset initial start position

	for (int j = 0; j < 300; j++)
	{
		if (vTMin >= (minMax.TMax - epsAdjust) || vTMin > closestPoint) 
			return false;

		MARCHES++;

		// Break condition if Voxel Info > 0, Solid voxel is found (Works on both Sparse and Dense grids)
		uint matIndex = getVoxelInfo(currentIndex, chunk);
		if (matIndex != 0)
		{
			closestPosition = chunk.MinExtent + vec3(currentIndex * chunk.VoxelSize + chunk.VoxelSize * 0.5);
			closestPoint = vTMin;
			closestMaterial = matIndex;
			return true;
		}

		// Step through grid by 1 voxel on X,Y,or Z
		if (tMax.x < tMax.y && tMax.x < tMax.z)
		{
			vTMin = tMax.x;
			tMax.x += tDelta.x;
			currentIndex.x += steps.x;
		}
		else if (tMax.y < tMax.z)
		{
			vTMin = tMax.y;
			tMax.y += tDelta.y;
			currentIndex.y += steps.y;
		}
		else
		{
			vTMin = tMax.z;
			tMax.z += tDelta.z;
			currentIndex.z += steps.z;
		}
	}
	
	return false;
}

struct ST
{
	uint Offset;
	vec3 T0;
	vec3 T1;
	uint Info;
	int Child;

} Stack[10];
int StackIndex = 0;

int firstIndex(vec3 t0, vec3 tM)
{
	bvec3 mask = greaterThan(vec3(max(max(t0.x, t0.y), t0.z)), tM);
	return (int(mask.z) << 0) | (int(mask.y) << 1) | (int(mask.x) << 2);
}

int nextIndex(int current, vec3 tM)
{
	int index = current;

	if (tM.x < tM.y && tM.x < tM.z)
		index |= 4;
	else if (tM.y < tM.z)
		index |= 2;
	else
		index |= 1;

	if (index == current) return 8;
	return index;
}

void childTs(int index, ST node, vec3 tM, inout ST child)
{
	switch (index)
	{
		case 0:
		{
			child.T0 = node.T0;
			child.T1 = tM;
			break;
		}
		case 1:
		{
			child.T0 = vec3(node.T0.xy, tM.z);
			child.T1 = vec3(tM.xy, node.T1.z);
			break;
		}
		case 2:
		{
			child.T0 = vec3(node.T0.x, tM.y, node.T0.z);
			child.T1 = vec3(tM.x, node.T1.y, tM.z);
			break;
		}
		case 3:
		{
			child.T0 = vec3(node.T0.x, tM.yz);
			child.T1 = vec3(tM.x, node.T1.yz);
			break;
		}
		case 4:
		{
			child.T0 = vec3(tM.x, node.T0.yz);
			child.T1 = vec3(node.T1.x, tM.yz);
			break;
		}
		case 5:
		{
			child.T0 = vec3(tM.x, node.T0.y, tM.z);
			child.T1 = vec3(node.T1.x, tM.y, node.T1.z);
			break;
		}
		case 6:
		{
			child.T0 = vec3(tM.xy, node.T0.z);
			child.T1 = vec3(node.T1.xy, tM.z);
			break;
		}
		case 7:
		{
			child.T0 = tM;
			child.T1 = node.T1;
			break;
		}
	}
}

// Stack based octree traversal inspired by Revelles
bool octDFSTraversal(Ray ray, Chunk chunk, TValue minMax, inout float closestPoint, inout vec3 closestPosition, inout uint closestMaterial)
{
	int mask = 0;
	if (ray.Direction.x < 0)
	{
		mask |= 4;
	}
	if (ray.Direction.y < 0)
	{
		mask |= 2;
	}
	if (ray.Direction.z < 0)
	{
		mask |= 1;
	}

	ST root;
	root.Offset = chunk.VoxelOffset;
	root.T0 = minMax.T0;
	root.T1 = minMax.T1;
	root.Info = 0;
	root.Child = -1;
	StackIndex = 0;

	vec3 tM = (root.T0 + root.T1) * 0.5;
	root.Child = firstIndex(root.T0, tM);
	Stack[StackIndex] = root;

	for (int i = 0; i < 250; i++)
	{
		MARCHES++;
		ST node = Stack[StackIndex];

		if (node.T1.x < 0. || node.T1.y < 0. || node.T1.z < 0.)
		{
			MARCHES = i;
			break;
		}

		ivec4 info = ivec4(unpackUnorm4x8(node.Info) * 255);
		if (info.w == 1)
		{
			if (info.x == 0)
			{
				StackIndex--;
				if (StackIndex < 0) break;
				continue;
			}

			closestPoint = max(max(node.T0.x, node.T0.y), node.T0.z);
			vec3 startPos = getRayPoint(ray, closestPoint + EPSILON * chunk.CamDistance * 0.001);
			ivec3 currentIndex = ivec3(floor((startPos - chunk.MinExtent) / chunk.VoxelSize));
			closestPosition = chunk.MinExtent + vec3(currentIndex * chunk.VoxelSize + chunk.VoxelSize * 0.5);
			closestMaterial = uint(info.x);
			return true;
		}

		tM = (node.T0 + node.T1) * 0.5;
		ST child;

		if (node.Child == 8)
		{
			StackIndex--;
			if (StackIndex < 0) break;
			continue;
		}

		child.Info = u_Voxels[node.Offset + (node.Child ^ mask)].Material;
		childTs(node.Child, node, tM, child);
		node.Child = nextIndex(node.Child, child.T1);

		tM = (child.T0 + child.T1) * 0.5;
		child.Child = firstIndex(child.T0, tM);
		info = ivec4(unpackUnorm4x8(child.Info) * 255);
		child.Offset = node.Offset + int(info.x + (info.y * 255) + (info.z * 255 * 255));
		Stack[StackIndex] = node;
		StackIndex++;
		Stack[StackIndex] = child;
	}
	
	return false;
}

bool findClosestVoxel(Ray ray, Chunk chunk, TValue minMax, inout float closestPoint, inout vec3 closestPosition, inout uint closestMaterial)
{
	switch (u_Algorithm)
	{
		case 0: return awDDATraversal(ray, chunk, minMax, closestPoint, closestPosition, closestMaterial); break;
		// About 1.5x Faster for VERY Sparse data with lots of empty space
		// case 1: return octDFSTraversal(ray, chunk, minMax, closestPoint, closestPosition, closestMaterial); break;
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
		TValue minMax = findBoxTminTmax(u_Chunks[i].MinExtent, u_Chunks[i].MaxExtent, ray);

		if (minMax.TMin <= minMax.TMax && minMax.TMin < closestPoint)
		{
			if (findClosestVoxel(ray, u_Chunks[i], minMax, closestPoint, closestPosition, closestMaterial))
			{
				closestChunk = i;
			}
		}
	}

	if (closestChunk < 0) return missHit(ray);

	return closestHitBox(ray, closestPosition, closestPoint, closestChunk, closestMaterial);
}

Ray getRay(ivec2 texelCoord)
{
	ivec2 screenSize = imageSize(imgOutput);
	vec2 sampleCoord = vec2(noise(sin(u_RandomSeed), -0.5, 0.5), noise(cos(u_RandomSeed), -0.5, 0.5));

	float horizonCoefficient = (float(texelCoord.x) * 2 - screenSize.x) / screenSize.x;
	float verticalCoefficient = (float(texelCoord.y) * 2 - screenSize.y) / screenSize.y;
	float horitzontalSample = (float(texelCoord.x + sampleCoord.x) * 2 - screenSize.x) / screenSize.x;
	float verticalSample = (float(texelCoord.y + sampleCoord.y) * 2 - screenSize.y) / screenSize.y;
	vec4 pixelSample = vec4(horizonCoefficient, verticalCoefficient, 1.0, 1.0) + vec4(horitzontalSample, verticalSample, 1.0, 1.0);

	Ray ray;
	ray.Origin = u_CamPosition.xyz;

	vec4 target = u_Projection * pixelSample;
	ray.Direction = normalize(vec3(u_View * vec4(normalize(vec3(target) / target.w), 0)));
	ray.InvDirection = 1 / ray.Direction;

	return ray;
}

vec3 shadowRay(Ray ray, vec3 throughput)
{
	RayTracePayload payload = traceBoxes(ray);

	if (payload.Distance < 0)
	{
		return u_BackgroundColor.xyz * throughput;
	}

	return vec3(0);
}

void main()
{
	ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
	EPSILON = float(1.0 / imageSize(imgOutput).y);

	vec3 light = vec3(0);
	vec3 sunDir = normalize(vec3(-1, -0.5, -1));

	ivec4 sampleSettings = ivec4(unpackUnorm4x8(u_SampleSettings) * 255);
	switch (u_RenderPass)
	{
		case 0: // Full Render Pass
			{
				for (int i = 0; i < sampleSettings.x; i++)
				{
					Ray ray = getRay(texelCoord);
					vec3 throughput = vec3(1);

					for (int j = 0; j < sampleSettings.y; j++)
					{
						RayTracePayload payload = traceBoxes(ray);
						Material mat = u_Materials[u_Chunks[payload.ObjIndex].MaterialIndex];

						if (payload.Distance >= 0)
						{
							vec3 albedo = unpackUnorm4x8(u_Chunks[payload.ObjIndex].Albedos[payload.AlbedoIndex]).xyz;
							throughput *= albedo;
							light += mat.Emissive * albedo;

							if (mat.Emissive > 0)
							{
								break;
							}

							// Ray to test for sun shadows
							Ray sRay;
							sRay.Origin = payload.Point + payload.Normal * 0.00001;
							sRay.Direction = -sunDir;
							sRay.InvDirection = 1 / sRay.Direction;
							light += shadowRay(sRay, throughput);

							// Diffuse lighting bounce
							ray.Origin = payload.Point + payload.Normal * 0.00001;
							ray.Direction = normalize(randInUnitSphere() + payload.Normal);
							ray.InvDirection = 1 / ray.Direction;
						}
						else
						{
							light += u_BackgroundColor.xyz * throughput;
							break;
						}
					}
				}

				light /= sampleSettings.x;

				break;
			}
		case 1: // Render Normals
			{
				RayTracePayload payload = traceBoxes(getRay(texelCoord));

				if (payload.Distance > 0)
				{
					vec3 color = vec3(payload.Normal);
					light = color;
				}
				break;
			}
		case 2: // Render Albedos
			{
				RayTracePayload payload = traceBoxes(getRay(texelCoord));
			
				if (payload.Distance > 0)
				{
					vec4 color = unpackUnorm4x8(u_Chunks[payload.ObjIndex].Albedos[payload.AlbedoIndex]);
					light += vec3(color);
				}
				break;
			}
		case 3: // Render Trace Difficuly
			{
				RayTracePayload payload = traceBoxes(getRay(texelCoord));

				if (payload.Distance > 0)
				{
					light = vec3(min(1.0, MARCHES / 200.0), 0, 0);
				}
				else
				{
					light = vec3(0, 0, min(1.0, MARCHES / 200.0));
				}
				break;
			}
	}

	imageStore(imgOutput, texelCoord, vec4(light, 1.0));
}