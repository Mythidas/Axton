#include "Camera.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float vFov, float nearClip, float farClip)
	: m_FOV(vFov), m_NearClip(nearClip), m_FarClip(farClip)
{
	m_Direction = Vector3(0.0f, 0.0f, -1.0f);
	m_Position = Vector3(0.0f, 0.0f, 3.0f);

	RecalculateView();
}

void Camera::OnUpdate()
{
	Vector2 mousePos = Input::GetMousePosition();
	Vector2 delta = (mousePos - m_LastMousePosition) * 0.002f;
	m_LastMousePosition = mousePos;

	if (!Input::IsKeyPressed(KeyCode::RightMouseButton))
	{
		Input::LockCursor(false);
		return;
	}

	Input::LockCursor(true);

	bool moved = false;

	constexpr Vector3 upDir(0.0f, 1.0f, 0.0f);
	Vector3 righDirection = Vector::Cross(m_Direction, upDir);

	float speed = 5.0f;

	if (Input::IsKeyPressed(KeyCode::W))
	{
		m_Position += m_Direction * speed * Time::GetDeltaTime();
		moved = true;
	}
	if (Input::IsKeyPressed(KeyCode::S))
	{
		m_Position -= m_Direction * speed * Time::GetDeltaTime();
		moved = true;
	}
	if (Input::IsKeyPressed(KeyCode::A))
	{
		m_Position -= righDirection * speed * Time::GetDeltaTime();
		moved = true;
	}
	if (Input::IsKeyPressed(KeyCode::D))
	{
		m_Position += righDirection * speed * Time::GetDeltaTime();
		moved = true;
	}
	if (Input::IsKeyPressed(KeyCode::Q))
	{
		m_Position -= upDir * speed * Time::GetDeltaTime();
		moved = true;
	}
	if (Input::IsKeyPressed(KeyCode::E))
	{
		m_Position += upDir * speed * Time::GetDeltaTime();
		moved = true;
	}

	if (delta.x != 0.0f || delta.y != 0.0f)
	{
		float pitchDelta = delta.y * GetRotationSpeed();
		float yawDelta = delta.x * GetRotationSpeed();

		Quaterion q = glm::normalize(glm::cross(glm::angleAxis(-pitchDelta, righDirection),
			glm::angleAxis(-yawDelta, upDir)));
		m_Direction = glm::rotate(q, m_Direction);

		moved = true;
	}

	if (moved)
	{
		RecalculateView();
		//RecalculateRayDirections();
	}
}

void Camera::OnResize(uint32_t width, uint32_t height)
{
	if (width == m_ViewportWidth && height == m_ViewportHeight) return;

	m_ViewportWidth = width;
	m_ViewportHeight = height;

	RecalculateProjection();
	//RecalculateRayDirections();
}

float Camera::GetRotationSpeed()
{
	return 0.3f;
}

void Camera::RecalculateProjection()
{
	m_Projection = glm::perspective(Mathf::ToRadians(m_FOV), (float)m_ViewportWidth / (float)m_ViewportHeight, m_NearClip, m_FarClip);
	m_InverseProjection = glm::inverse(m_Projection);
}

void Camera::RecalculateView()
{
	m_View = glm::lookAt(m_Position, m_Position + m_Direction, Vector3(0.0f, 1.0f, 0.0f));
	m_InverseView = glm::inverse(m_View);
}

void Camera::RecalculateRayDirections()
{
	m_RayDirections.resize((size_t)(m_ViewportWidth * m_ViewportHeight));

	for (uint32_t y = 0; y < m_ViewportHeight; y++)
	{
		for (uint32_t x = 0; x < m_ViewportWidth; x++)
		{
			Vector2 coord = { (float)x / (float)m_ViewportWidth, (float)y / (float)m_ViewportHeight };
			coord = coord * 2.0f - 1.0f; // -1, 1

			Vector4 target = m_InverseProjection * Vector4(coord.x, coord.y, 1.0f, 1.0f);
			Vector3 rayDirection = Vector3(m_InverseView * Vector4(Vector::UnitVector(Vector3(target) / target.w), 0));
			m_RayDirections[(size_t)(x + y * m_ViewportWidth)] = rayDirection;
		}
	}
}
