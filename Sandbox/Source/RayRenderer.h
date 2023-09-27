#pragma once

#include "Axton.h"

using namespace Axton;

class RayRenderer
{
public:
	void Render();
	void Resize(uint32_t width, uint32_t height);

	Ref<Texture2D> GetFinalImage() const { return m_FinalImage; }

	Vector3 TempColor;
	Vector3 TempLight;

private:
	Vector4 PerPixel(Vector2 coord);

private:
	Ref<Texture2D> m_FinalImage;
	uint32_t* m_ImageData;
};