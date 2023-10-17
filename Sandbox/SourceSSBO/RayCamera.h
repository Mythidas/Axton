#pragma once

#include "Axton.h"

using namespace Axton;

class RayCamera : public Camera
{
	struct Buffer
	{
		Vector4 Position{ 0.0f };
		Vector4 Direction{ 0.0f };
		Matrix4 View{ 0.0f };
		Matrix4 Projection{ 0.0f };
		float RandomSeed;
		uint32_t PixelSamples{ 5 };
		uint32_t RenderPass;
	};

public:
	enum class RenderPass
	{
		Full = 0,
		Normals,
		Albedo
	};

public:
	RayCamera(const Camera::Specs& specs);

	void OnUpdate();
	Ray GetRay() const;

	void SetRenderPass(RenderPass pass) { m_RenderPass = pass; }

	virtual void OnResize(uint32_t width, uint32_t height) override;
	virtual void RecalculateProjection() override;
	virtual void RecalculateView() override;

	float GetRotationSpeed();

private:
	void ProcessMovement();

private:
	RenderPass m_RenderPass{ RenderPass::Full };
	Vector2 m_LastMousePosition{ 0.0f };
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	Ref<UniformBuffer> m_CameraTransform;
};