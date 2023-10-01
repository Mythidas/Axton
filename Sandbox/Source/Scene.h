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
	alignas(16) Vector4 Position;
	float Radius;
	int MatIndex;
};

class Scene
{
public:
	Scene(uint32_t maxSpheres);

	void AddSphere(Sphere sphere);
	void AddMaterial(Material material);
	void SyncBuffers();

	std::vector<Sphere> Spheres;
	std::vector<Material> Materials;

	Ref<StorageBuffer> SphereStorage;
	Ref<StorageBuffer> MaterialStorage;
};