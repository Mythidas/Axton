#include "RayRenderer.h"

namespace Utils
{
	static uint32_t ConvertToRGBA(const Vector4& color)
	{
		return 0xff000000 | ((uint8_t)(color.a * 255.0f) << 24) | ((uint8_t)(color.b * 255.0f) << 16) | ((uint8_t)(color.g * 255.0f) << 8) | ((uint8_t)(color.r * 255.0f));
	}
}

void RayRenderer::Render()
{
	Timer timer("RayRenderer::Render");

	float aspectRatio = (float)m_FinalImage->GetWidth() / (float)m_FinalImage->GetHeight();

	for (int y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (int x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			Vector2 coord = { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };
			coord = coord * 2.0f - 1.0f;
			coord.x *= aspectRatio;
			Vector4 color = Vector::Clamp(PerPixel(coord), Vector4(0.0f), Vector4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);
		}
	}

	m_FinalImage->SetData(m_ImageData, m_FinalImage->GetWidth() * m_FinalImage->GetHeight() * 4);
}

void RayRenderer::Resize(uint32_t width, uint32_t height)
{
	if (!m_FinalImage || width != m_FinalImage->GetWidth() || height != m_FinalImage->GetHeight())
	{
		m_FinalImage = Texture2D::Create({ width, height });
		delete[] m_ImageData;
		m_ImageData = new uint32_t[width * height];
		m_FinalImage->SetData(m_ImageData, width * height * 4);
	} 
}

Vector4 RayRenderer::PerPixel(Vector2 coord)
{
	// (bx^2 + by^2 + bz^2)t^2 + (2(axbx + ayby + azbz))t + (ax^2 + ay^2 + az^2 - r^2) = 0
	// a = Ray Origin
	// b = Ray Direction
	// r = radius
	// t = Hit Distance

	Vector3 camera = { 0.0f, 0.0f, 1.0f };
	Vector3 center = { 0.0f, 0.0f, -1.0f };

	Ray ray(camera, { coord.x, coord.y, -1.0f});
	Vector3 oc = ray.Origin - center;

	float a = Vector::Dot(ray.Direction, ray.Direction);
	float b = 2.0f * Vector::Dot(ray.Direction, oc);
	float c = Vector::Dot(oc, oc) - 0.5f * 0.5f;

	float discriminant = b * b - 4.0f * a * c;

	if (discriminant >= 0.0f)
	{
		float t[2] =
		{
			(-b - sqrt(discriminant)) / (2.0f * a),
			(-b + sqrt(discriminant)) / (2.0f * a)
		};
		
		for (int i = 0; i < 2; i++)
		{
			if (t[i] > 0.0f)
			{
				Vector3 point = ray.GetPoint(t[i]);
				Vector3 normal = Vector::UnitVector(point - center);
				Vector3 color = 0.5f * (normal + 1.0f);
				return Vector4(color, 1.0f);
			}
		}
	}

	return Vector4(-coord.y * 0.9f + 0.2f, -coord.y * 0.9f + 0.2f, 0.9f, 1.0f);
}
