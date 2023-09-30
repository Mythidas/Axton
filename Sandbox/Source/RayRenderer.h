#pragma once

#include "Axton.h"

#include "Scene.h"
#include "Camera.h"

using namespace Axton;

struct RayHitPayload
{
	Vector3 Point{ 0.0f };
	Vector3 Normal{ 0.0f };
	float Distance{ 0.0f };
	int ObjectIndex{ -1 };
};

class RayRenderer
{
public:
	void Render(const Camera& camera, const Scene& scene);
	void Resize(uint32_t width, uint32_t height);

	Ref<Texture2D> GetFinalImage() const { return m_FinalImage; }

	Vector3 TempLight;

private:
	Vector4 PerPixel(uint32_t x, uint32_t y); // RayGen Shader
	RayHitPayload TraceRay(const Ray& ray);
	RayHitPayload ClosestHit(const Ray& ray, float hitDistance, int objectIndex);
	RayHitPayload Miss(const Ray& ray);

private:
	Ref<Texture2D> m_FinalImage{ nullptr };
	uint32_t* m_ImageData{ nullptr };

	const Scene* m_ActiveScene = nullptr;
	const Camera* m_ActiveCamera = nullptr;
};