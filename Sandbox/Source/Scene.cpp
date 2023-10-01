#include "Scene.h"

Scene::Scene(uint32_t maxSpheres)
{
	SphereStorage = StorageBuffer::Create(maxSpheres * sizeof(Sphere), BufferUsage::DYNAM_DRAW, 2);
	MaterialStorage = StorageBuffer::Create(maxSpheres * sizeof(Material), BufferUsage::DYNAM_DRAW, 3);
}

void Scene::AddSphere(Sphere sphere)
{
	Spheres.push_back(sphere);
}

void Scene::AddMaterial(Material material)
{
	Materials.push_back(material);
}

void Scene::SyncBuffers()
{
	SphereStorage->SetData(Spheres.data(), Spheres.size() * sizeof(Sphere));
	MaterialStorage->SetData(Materials.data(), Materials.size() * sizeof(Material));
}
