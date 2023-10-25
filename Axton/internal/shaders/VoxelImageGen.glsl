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
	uint u_PixelSamples;
	uint u_SampleDepth;
	uint u_RenderPass;
};

struct Chunk
{
	vec3 MinExtent;
	vec3 MaxExtent;
	ivec3 GridSize;
	float VoxelSize;
	float CamDistance;
	uint VoxelOffset;
	uint MaterialIndex;
	uint[255] Albedos;
};

struct Voxel
{
	uint MatIndex;
};

struct Material
{
	float Specular;
	float Metallic;
	float Roughness;
	float Emissive;
	vec3 EmissiveHue;
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

float lerp(float a, float b, float value)
{
	return a * (1.0 - value) + (b * value);
}

const float PI = 3.14159265359;
const float PHI = 1.61803398874989484820459; // Golden Ratio 

float gold_noise(in vec2 xy, in float seed)
{
	return fract(tan(distance(xy*PHI, xy)*seed)*xy.x);
}

float custom_noise(vec2 st)
{
	return fract(sin(dot(st, vec2(12.9898, 78.233))) * 43758.5453123);
}

float simple_noise(vec2 xy)
{
  return fract(52.9829189 * fract(xy.x * 0.06711056 + xy.y * 0.00583715));
}

float noise(float seed)
{
	return lerp(0, 1, custom_noise(gl_GlobalInvocationID.xy + seed));
}

float noise(float seed, float low, float high)
{
	return lerp(low, high, custom_noise(gl_GlobalInvocationID.xy + seed));
}

vec3 randomSpherePoint(vec3 rand) {
	float ang1 = (rand.x + 1.0) * PI; // [-1..1) -> [0..2*PI)
	float u = rand.y; // [-1..1), cos and acos(2v-1) cancel each other out, so we arrive at [-1..1)
	float u2 = u * u;
	float sqrt1MinusU2 = sqrt(1.0 - u2);
	float x = sqrt1MinusU2 * cos(ang1);
	float y = sqrt1MinusU2 * sin(ang1);
	float z = u;
	return vec3(x, y, z);
}

vec3 randomHemispherePoint(vec3 rand, vec3 n) {
	/**
	* Generate random sphere point and swap vector along the normal, if it
	* points to the wrong of the two hemispheres.
	* This method provides a uniform distribution over the hemisphere, 
	* provided that the sphere distribution is also uniform.
	*/
	vec3 v = randomSpherePoint(rand);
	return normalize(v * sign(dot(v, n)));
}

uint collapseIndex(ivec3 index, Chunk chunk)
{
	return uint(((chunk.GridSize.x * chunk.GridSize.y * index.z) + (chunk.GridSize.x * index.y) + index.x));	
}

uint getVoxelInfo(ivec3 index, Chunk chunk, int lod)
{
	int currentOffset = int(chunk.VoxelOffset);

	int levels = int(log2(chunk.GridSize.x));
	for (int i = 0; i < levels - lod; i++)
	{
		int childIndex = 0;
		int halfSize = 1 << (levels - i - 1);

		if (index.x >= halfSize)
		{
			childIndex |= 1;
			index.x -= halfSize;
		}
		if (index.y >= halfSize)
		{
			childIndex |= 2;
			index.y -= halfSize;
		}
		if (index.z >= halfSize)
		{
			childIndex |= 4;
			index.z -= halfSize;
		}

		ivec4 info = ivec4(unpackUnorm4x8(u_Voxels[currentOffset + childIndex].MatIndex) * 255);
		if (info.w == 1)
		{
			return uint(info.x);
		}
		else
		{
			currentOffset += int(info.x + (info.y * 255) + (info.z * 255 * 255));
		}
	}
	
	return ivec4(unpackUnorm4x8(u_Voxels[currentOffset].MatIndex) * 255).w;
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
	int Marches;
};

RayTracePayload missHit(Ray ray, int marches)
{
	RayTracePayload payload;
	payload.Distance = -1;
	payload.Marches = marches;
	return payload;
}

vec3 getVoxelNormal(vec3 center, vec3 hit)
{
	vec3 diff = abs(hit - center);
	vec3 diff1 = hit - center;
	if (diff.x > diff.y && diff.x > diff.z)
		return vec3(sign(-diff1.x), 0, 0);
	else if (diff.y > diff.z)
		return vec3 (0, sign(diff1.y), 0);
	else
		return vec3(0, 0, sign(-diff1.z));
}

RayTracePayload closestHitBox(Ray ray, vec3 position, float hitDistance, int objIndex, uint albedo, int marches)
{
	RayTracePayload payload;
	payload.Distance = hitDistance;
	payload.ObjIndex = objIndex;
	payload.AlbedoIndex = albedo;
	payload.Point = getRayPoint(ray, hitDistance);
	payload.Normal = getVoxelNormal(position, payload.Point);
	payload.Marches = marches;
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
	value.TMax = 100000000;

	vec3 t0s = (minExtent - ray.Origin) * ray.InvDirection;
	vec3 t1s = (maxExtent - ray.Origin) * ray.InvDirection;

	value.T0 = min(t0s, t1s);
	value.T1 = max(t0s, t1s);

	value.TMin = max(value.TMin, max(value.T0[0], max(value.T0[1], value.T0[2])));
	value.TMax = min(value.TMax, min(value.T1[0], min(value.T1[1], value.T1[2])));

	return value;
}

// Amanatides & Woo 3DDA Traversal
bool awDDATraversal(Ray ray, Chunk chunk, TValue minMax, inout int marches, inout float closestPoint, inout vec3 closestPosition, inout uint closestMaterial)
{
	/// INITIALIZATION ///
	// Find the Voxel where the ray starts
	vec3 startPos = getRayPoint(ray, minMax.TMin + 0.0003);
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
	for (int j = 0; j < chunk.GridSize.x * chunk.GridSize.y * chunk.GridSize.z; j++)
	{
		if (currentIndex.x < 0 || currentIndex.x >= chunk.GridSize.x
			|| currentIndex.y < 0 || currentIndex.y >= chunk.GridSize.y
			|| currentIndex.z < 0 || currentIndex.z >= chunk.GridSize.z)
		{
			break;
		}
		marches++;

		// Break condition if Voxel Info > 0, Solid voxel is found (Works on both Sparse and Dense grids)
		uint matIndex = getVoxelInfo(currentIndex, chunk, 0);
		if (matIndex != 0)
		{
			closestPosition = chunk.MinExtent + vec3(currentIndex * chunk.VoxelSize + chunk.VoxelSize * 0.5);
			closestPoint = vTMin;
			closestMaterial = matIndex;
			return true;
		}

		vTMin = min(min(tMax.x, tMax.y), tMax.z);
		if (vTMin > closestPoint)
			return false;

		// Step through grid by 1 voxel on X,Y,or Z
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

// Modified from Amantides & Woo for use in Octrees
bool octDDATraversal(Ray ray, Chunk chunk, TValue minMax, inout int marches, inout float closestPoint, inout vec3 closestPosition, inout uint closestMaterial)
{
	return false;
}

bool findClosestVoxel(Ray ray, Chunk chunk, TValue minMax, inout int marches, inout float closestPoint, inout vec3 closestPosition, inout uint closestMaterial)
{
	int method = 0;
	if (method == 0)
		return awDDATraversal(ray, chunk, minMax, marches, closestPoint, closestPosition, closestMaterial);
	else if (method == 1)
		return octDDATraversal(ray, chunk, minMax, marches, closestPoint, closestPosition, closestMaterial);
}

RayTracePayload traceBoxes(Ray ray)
{
	float closestPoint = 100000000;
	int closestChunk = -1;
	vec3 closestPosition = vec3(0);
	uint closestMaterial = -1;
	int marches = 0;

	for (int i = 0; i < u_Chunks.length(); i++)
	{
		TValue minMax = findBoxTminTmax(u_Chunks[i].MinExtent, u_Chunks[i].MaxExtent, ray);

		if (minMax.TMin <= minMax.TMax && minMax.TMin < closestPoint)
		{
			if (findClosestVoxel(ray, u_Chunks[i], minMax, marches, closestPoint, closestPosition, closestMaterial))
			{
				closestChunk = i;
			}
		}
	}

	if (closestChunk < 0) return missHit(ray, marches);

	return closestHitBox(ray, closestPosition, closestPoint, closestChunk, closestMaterial, marches);
}

Ray getRay(ivec2 texelCoord)
{
	ivec2 screenSize = imageSize(imgOutput);
	vec2 sampleCoord = vec2(noise(u_RandomSeed, -0.5, 0.5), noise(u_RandomSeed + 1, -0.5, 0.5));

	float horizonCoefficient = (float(texelCoord.x) * 2 - screenSize.x) / screenSize.x;
	float verticalCoefficient = (float(texelCoord.y) * 2 - screenSize.y) / screenSize.y;
	float horitzontalSample = (float(texelCoord.x + sampleCoord.x) * 2 - screenSize.x) / screenSize.x;
	float verticalSample = (float(texelCoord.y + sampleCoord.y) * 2 - screenSize.y) / screenSize.y;
	vec4 pixelSample = vec4(horizonCoefficient, verticalCoefficient, 1.0, 1.0) + vec4(horitzontalSample, verticalSample, 1.0, 1.0);
	//vec4 pixelSample = vec4(horizonCoefficient, verticalCoefficient, 1.0, 1.0);

	Ray ray;
	ray.Origin = u_CamPosition.xyz;

	vec4 target = u_Projection * pixelSample;
	ray.Direction = normalize(vec3(u_View * vec4(normalize(vec3(target) / target.w), 0)));
	ray.InvDirection = 1 / ray.Direction;

	return ray;
}

vec3 randV3(float low, float high)
{
	return vec3(noise(u_RandomSeed), noise(u_RandomSeed + 1), noise(u_RandomSeed + 2));
}

vec3 randInUnitSphere()
{
	return normalize(randV3(-1, 1));
}

void main()
{
	vec3 light = vec3(0);
	ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

	switch (u_RenderPass)
	{
		case 0: // Full Render Pass
			{
				float samples = 1;
				for (int i = 0; i < u_PixelSamples; i++)
				{
					Ray ray = getRay(texelCoord);
					vec3 throughput = vec3(1);

					samples++;
					for (int j = 0; j < u_SampleDepth; j++)
					{
						RayTracePayload payload = traceBoxes(ray);
						Material mat = u_Materials[u_Chunks[payload.ObjIndex].MaterialIndex];

						if (payload.Distance > 0)
						{
							vec3 albedo = unpackUnorm4x8(u_Chunks[payload.ObjIndex].Albedos[payload.AlbedoIndex]).xyz;
							throughput *= albedo;
							light += mat.Emissive * albedo;

							ray.Origin = payload.Point + payload.Normal * 0.0001;
							ray.Direction = normalize(randInUnitSphere() + payload.Normal + 0.00001);
							ray.InvDirection = 1 / ray.Direction;
						}
						else
						{
							light += u_BackgroundColor.xyz * throughput;
							samples += j - 1;
							break;
						}
					}
				}

				light /= samples;

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
					light = vec3(min(1.0, payload.Marches / 300.0), 0, 0);
				}
				else
				{
					light = vec3(0, 0, min(1.0, payload.Marches / 200.0));
				}
				break;
			}
	}

	imageStore(imgOutput, texelCoord, vec4(light, 1.0));
}