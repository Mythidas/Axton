#pragma once

#include "Axton.h"

using namespace Axton;

struct Material
{
	Vector3 Albedo;
	float Roughness;
};

struct Sphere
{
	Vector3 Position;
	float Radius;
	uint32_t MatIndex;
};

class Scene
{
public:
	void AddSphere(Sphere sphere);
	void AddMaterial(Material material);

	std::vector<Sphere> Spheres;
	std::vector<Material> Materials;
};