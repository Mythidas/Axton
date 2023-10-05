#version 430 core

layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
layout (rgba32f, binding = 0) uniform image2D imgOutput;
layout (r8, binding = 1) uniform image3D imgInput;

struct Box
{
	vec3 Position;
	vec3 Extents;
	int MatIndex;
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

vec3 getRayPoint(Ray ray, float t)
{
	return ray.Origin + ray.Direction * (t + 0.00001);
}

// Data Buffers
layout(std140, binding = 0) uniform Camera
{
	vec4 u_CamPosition;
	vec4 u_CamDirection;
	mat4 u_View;
	mat4 u_Projection;
};
layout (std430, binding = 2) readonly buffer boxStorage
{
	Box[] u_Boxes;
};
layout (std430, binding = 3) readonly buffer materialStorage
{
	Material[] u_Materials;
};

RayTracePayload missHit(Ray ray)
{
	RayTracePayload payload;
	payload.Distance = -1;
	return payload;
}

RayTracePayload closestHitBox(Box box, Ray ray, float hitDistance, int objIndex)
{
	RayTracePayload payload;
	payload.Distance = hitDistance;
	payload.ObjIndex = objIndex;
	payload.Point = getRayPoint(ray, hitDistance);

	vec3 minExtent = box.Position - box.Extents;
	vec3 maxExtent = box.Position + box.Extents;
	vec3 p = payload.Point - box.Position;
	float dx = abs((minExtent.x - maxExtent.x) / 2);
	float dy = abs((minExtent.y - maxExtent.y) / 2);
	float dz = abs((minExtent.z - maxExtent.z) / 2);
	payload.Normal = normalize(vec3(int(p.x / dx), int(p.y / dy), int(p.z / dz)));
	return payload;
}

vec2 findBoxTminTmax(Box box, Ray ray)
{
	vec3 minExtent = box.Position - box.Extents;
	vec3 maxExtent = box.Position + box.Extents;

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
	Box returnBox;
	float closestPoint = 5000;
	int closestBox = -1;
	for (int i = 0; i < u_Boxes.length(); i++)
	{
		vec2 minMax = findBoxTminTmax(u_Boxes[i], ray);
		float tmin = minMax.x;
		float tmax = minMax.y;

		if (tmin <= tmax && tmin < closestPoint)
		{
			vec3 minExtent = u_Boxes[i].Position - u_Boxes[i].Extents;
			vec3 stepSize = abs(1 / ray.Direction);
			vec3 currentPos = getRayPoint(ray, tmin);
			ivec3 index = ivec3(floor((currentPos - minExtent) / 1));

			for (int j = 0; j < 100; j++)
			{
				float solid = imageLoad(imgInput, index).r;
				if (solid > 0)
				{	
					closestBox = i;
					break;
				}

				if (stepSize.x < stepSize.y && stepSize.x < stepSize.z)
				{
					currentPos.x += 1 * sign(ray.Direction).x;
					index.x += 1;
				}
				else if (stepSize.y < stepSize.z)
				{
					currentPos.y += 1 * sign(ray.Direction).y;
					index.y += 1;
				}
				else
				{
					currentPos.z += 1 * sign(ray.Direction).z;
					index.z += 1;
				}

				if (any(lessThan(index, ivec3(0))) || any(greaterThanEqual(index, ivec3(4)))) break;
			}
		}
	}

	if (closestBox < 0) return missHit(ray);

	return closestHitBox(returnBox, ray, closestPoint, closestBox);
}

void main()
{
	vec4 finalColor = vec4(0, 0, 0, 1);
	ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

	// Calculating the current x and y of the pixels
	ivec2 screenSize = imageSize(imgOutput);
	float horizonCoefficient = (float(texelCoord.x) * 2 - screenSize.x) / screenSize.x;
	float verticalCoefficient = (float(texelCoord.y) * 2 - screenSize.y) / screenSize.y;
	vec4 pixelSample = vec4(horizonCoefficient, verticalCoefficient, 1.0, 1.0);

	// Ray
	Ray ray;
	ray.Origin = u_CamPosition.xyz;

	vec4 target = u_Projection * pixelSample;
	ray.Direction = vec3(u_View * vec4(normalize(vec3(target) / target.w), 0));

	RayTracePayload payload = traceBoxes(ray);

	if (payload.Distance > 0)
	{
		finalColor = u_Materials[u_Boxes[payload.ObjIndex].MatIndex].Albedo;
	}
	else
	{
		finalColor = vec4(1) * vec4(0.5, 0.7, 1.0, 1.0) * 0.5;
	}

	imageStore(imgOutput, texelCoord, finalColor);
}