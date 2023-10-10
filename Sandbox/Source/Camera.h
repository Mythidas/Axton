#pragma once

#include "Axton.h"

using namespace Axton;

struct CameraTransformData
{
	Vector4 Position{ 0.0f };
	Vector4 Direction{ 0.0f };
	Matrix4 View{ 0.0f };
	Matrix4 Projection{ 0.0f };
};

class RayCamera : public Camera
{
public:
	RayCamera(const Camera::Specs& specs);

	void OnUpdate();

	virtual void OnResize(uint32_t width, uint32_t height) override;
	virtual void RecalculateProjection() override;
	virtual void RecalculateView() override;

	float GetRotationSpeed();

private:
	void ProcessMovement();

private:
	Ref<UniformBuffer> m_CameraTransform;
	CameraTransformData m_CameraTransformData{};

	Vector2 m_LastMousePosition{ 0.0f };

	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
};