#pragma once

#include "Axton.h"

using namespace Axton;

struct Material
{
	alignas(16) Vector4 Albedo;
	float Roughness;
};

struct Sphere
{
	alignas(16) Vector3 Position;
	float Radius;
	int MatIndex;
};

struct Box
{
	alignas(16) Vector3 Position;
	alignas(16) Vector3 Extents;
	int MatIndex;
};

struct Light
{
	alignas(16) Vector3 Position;
	alignas(16) Vector3 Direction;
	alignas(16) Vector4 Color;
};

class Scene
{
public:
	Scene(uint32_t maxSpheres);

	void AddSphere(Sphere sphere);
	void AddBox(Box box);
	void AddLight(Light light);
	void AddMaterial(Material material);
	void SyncBuffers();

	std::vector<Sphere> Spheres;
	std::vector<Box> Boxes;
	std::vector<Light> Lights;
	std::vector<Material> Materials;

	Ref<StorageBuffer> SphereStorage;
	Ref<StorageBuffer> BoxStorage;
	Ref<StorageBuffer> LightStorage;
	Ref<StorageBuffer> MaterialStorage;
};