#pragma once

#include "Axton.h"

using namespace Axton;

class Camera
{
public:
	Camera(float vFov, float nearClip, float farClip);

	void OnUpdate();
	void OnResize(uint32_t width, uint32_t height);

	const Matrix4& GetProjection() const { return m_Projection; }
	const Matrix4& GetInverseProjection() const { return m_InverseProjection; }
	const Matrix4& GetView() const { return m_View; }
	const Matrix4& GetInverseView() const { return m_InverseView; }

	Vector3 GetPosition() const { return m_Position; }
	Vector3 GetDirection() const { return m_Direction; }

	const std::vector<Vector3>& GetRayDirections() const { return m_RayDirections; }
	
	float GetRotationSpeed();

private:
	void RecalculateProjection();
	void RecalculateView();
	void RecalculateRayDirections();

private:
	Matrix4 m_Projection{ 1.0f };
	Matrix4 m_View{ 1.0f };
	Matrix4 m_InverseProjection{ 1.0f };
	Matrix4 m_InverseView{ 1.0f };

	float m_FOV = 45;
	float m_NearClip = 0.1f;
	float m_FarClip = 100.0f;

	Vector3 m_Position{ 0.0f };
	Vector3 m_Direction{ 0.0f };

	std::vector<Vector3> m_RayDirections;

	Vector2 m_LastMousePosition{ 0.0f };

	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
};