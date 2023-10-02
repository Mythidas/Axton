#include "Scene.h"

Scene::Scene(uint32_t maxSpheres)
{
	SphereStorage = StorageBuffer::Create(maxSpheres * sizeof(Sphere), BufferUsage::DYNAM_DRAW, 2);
	BoxStorage = StorageBuffer::Create(maxSpheres * sizeof(Box), BufferUsage::DYNAM_DRAW, 3);
	LightStorage = StorageBuffer::Create(maxSpheres * sizeof(Lights), BufferUsage::DYNAM_DRAW, 4);
	MaterialStorage = StorageBuffer::Create(maxSpheres * sizeof(Material), BufferUsage::STATIC_DRAW, 5);
}

void Scene::AddSphere(Sphere sphere)
{
	Spheres.push_back(sphere);
}

void Scene::AddBox(Box box)
{
	Boxes.push_back(box);
}

void Scene::AddLight(Light light)
{
	Lights.push_back(light);
}

void Scene::AddMaterial(Material material)
{
	Materials.push_back(material);
}

void Scene::SyncBuffers()
{
	SphereStorage->SetData(Spheres.data(), Spheres.size() * sizeof(Sphere));
	BoxStorage->SetData(Boxes.data(), Boxes.size() * sizeof(Box));
	LightStorage->SetData(Lights.data(), Lights.size() * sizeof(Light));
	MaterialStorage->SetData(Materials.data(), Materials.size() * sizeof(Material));
}
