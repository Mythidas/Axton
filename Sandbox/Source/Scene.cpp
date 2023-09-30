#include "Scene.h"

void Scene::AddSphere(Sphere sphere)
{
	Spheres.push_back(sphere);
}

void Scene::AddMaterial(Material material)
{
	Materials.push_back(material);
}