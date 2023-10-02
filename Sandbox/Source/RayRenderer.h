#pragma once

#include "Axton.h"

#include "Scene.h"
#include "Camera.h"

using namespace Axton;

class RayRenderer
{
public:
	RayRenderer();

	void Render(const Camera& camera, const Scene& scene);
	void Resize(uint32_t width, uint32_t height);

	Ref<Image> GetFinalImage() const { return m_FinalImage; }

private:
	Ref<Image> m_FinalImage{ nullptr };
	Ref<ComputeShader> m_ComputeShader;

	const Scene* m_ActiveScene = nullptr;
	const Camera* m_ActiveCamera = nullptr;
};