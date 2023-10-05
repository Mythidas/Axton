#include "Scene.h"

Scene::Scene(uint32_t maxSpheres)
{
	BoxStorage = StorageBuffer::Create(maxSpheres * sizeof(Box), BufferUsage::DYNAM_DRAW, 2);
	MaterialStorage = StorageBuffer::Create(maxSpheres * sizeof(Material), BufferUsage::STATIC_DRAW, 3);
}

void Scene::AddBox(Box box)
{
	Boxes.push_back(box);
}

void Scene::AddMaterial(Material material)
{
	Materials.push_back(material);
}

void Scene::SyncBuffers()
{
	BoxStorage->SetData(Boxes.data(), Boxes.size() * sizeof(Box));
	MaterialStorage->SetData(Materials.data(), Materials.size() * sizeof(Material));
}
