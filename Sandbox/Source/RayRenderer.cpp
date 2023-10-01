#include "RayRenderer.h"

namespace Utils
{
	static uint32_t ConvertToRGBA(const Vector4& color)
	{
		return 0xff000000 | ((uint8_t)(color.a * 255.0f) << 24) | ((uint8_t)(color.b * 255.0f) << 16) | ((uint8_t)(color.g * 255.0f) << 8) | ((uint8_t)(color.r * 255.0f));
	}
}

RayRenderer::RayRenderer()
{
	m_FinalImage = Image::Create({ 1280, 720 });
	m_ComputeShader = ComputeShader::Create(
		"C:\\Programming\\Axton\\Axton\\internal\\shaders\\RayGen.glsl");
}

void RayRenderer::Render(const Camera& camera, const Scene& scene)
{
	m_ActiveScene = &scene;
	m_ActiveCamera = &camera;

	const Vector3& rayOrigin = camera.GetPosition();

	m_ComputeShader->Dispatch(m_FinalImage->GetWidth(), m_FinalImage->GetHeight(), 1);
	m_ComputeShader->Barrier();
}

void RayRenderer::Resize(uint32_t width, uint32_t height)
{
	if (width != m_FinalImage->GetWidth() || height != m_FinalImage->GetHeight())
	{
		m_FinalImage->Resize(width, height);
	} 
}

// Old CPU RayTraced Code
//Vector4 RayRenderer::PerPixel(uint32_t x, uint32_t y)
//{
//	Vector3 lightDir = Vector::UnitVector(TempLight);
//	const Vector3& rayDirection = m_ActiveCamera->GetRayDirections()[(size_t)(x + y * m_FinalImage->GetWidth())];
//	Ray ray(m_ActiveCamera->GetPosition(), rayDirection);
//
//	Vector3 color(0.0f);
//
//	float bounceStrength = 0.5f;
//	int bounces = 2;
//	for (int i = 0; i < bounces; i++)
//	{
//		RayHitPayload payload = TraceRay(ray);
//
//		if (payload.Distance < 0.0f)
//		{
//			color += Vector3(0.0f) * bounceStrength;
//			bounceStrength *= 0.7f;
//			break;
//		}
//
//		const Sphere& sphere = m_ActiveScene->Spheres[payload.ObjectIndex];
//		const Material& material = m_ActiveScene->Materials[sphere.MatIndex];
//
//		float d = Mathf::Clamp::Float(Vector::Dot(payload.Normal, -lightDir), 0.0f, Mathf::Infinity::Float);
//		color += material.Albedo * d * bounceStrength;
//
//		bounceStrength *= 0.7f;
//
//		ray.Origin = payload.Point + (payload.Normal * 0.0001f);
//		ray.Direction = Vector::Reflect(ray.Direction, payload.Normal);
//	}
//
//	return Vector4(color, 1.0f);
//}
//
//RayHitPayload RayRenderer::TraceRay(const Ray& ray)
//{
//	int closestSphere = -1;
//	float closest = Mathf::Infinity::Float;
//	for (int i = 0; i < m_ActiveScene->Spheres.size(); i++)
//	{
//		const Sphere& sphere = m_ActiveScene->Spheres[i];
//		Vector3 oc = ray.Origin - sphere.Position;
//
//		float a = Vector::Dot(ray.Direction, ray.Direction);
//		float b = 2.0f * Vector::Dot(ray.Direction, oc);
//		float c = Vector::Dot(oc, oc) - sphere.Radius * sphere.Radius;
//
//		float discriminant = b * b - 4.0f * a * c;
//
//		if (discriminant >= 0.0f)
//		{
//			float distance = (-b - sqrt(discriminant)) / (2.0f * a);
//
//			if (distance > 0.0f && distance < closest)
//			{
//				closest = distance;
//				closestSphere = i;
//			}
//		}
//	}
//
//	if (closestSphere < 0) 
//		return Miss(ray);
//
//	return ClosestHit(ray, closest, closestSphere);
//}
//
//RayHitPayload RayRenderer::ClosestHit(const Ray& ray, float hitDistance, int objectIndex)
//{
//	RayHitPayload payload;
//	payload.Distance = hitDistance;
//	payload.ObjectIndex = objectIndex;
//	payload.Point = ray.GetPoint(hitDistance);
//	payload.Normal = Vector::UnitVector(payload.Point - m_ActiveScene->Spheres[objectIndex].Position);
//	return payload;
//}
//
//RayHitPayload RayRenderer::Miss(const Ray& ray)
//{
//	RayHitPayload payload;
//	payload.Distance = -1;
//	return payload;
//}