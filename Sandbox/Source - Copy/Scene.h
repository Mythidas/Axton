#pragma once

#include "Axton.h"

using namespace Axton;

struct Material
{
	alignas(16) Vector4 Albedo;
	float Roughness;
};

struct Box
{
	alignas(16) Vector3 Position;
	alignas(16) Vector3 Extents;
	int MatIndex;
};

class Scene
{
public:
	Scene(uint32_t maxSpheres);

	void AddBox(Box box);
	void AddMaterial(Material material);
	void SyncBuffers();

	std::vector<Box> Boxes;
	std::vector<Material> Materials;

	Ref<StorageBuffer> BoxStorage;
	Ref<StorageBuffer> MaterialStorage;
};