#include "RayCamera.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

RayCamera::RayCamera(const Camera::Specs& specs)
{
	m_Specs = specs;
	m_Direction = Vector3(0.0f, 0.0f, 1.0f);
	m_Position = Vector3(0.0f, 0.0f, -3.0f);

	RecalculateView();

	m_CameraTransform = UniformBuffer::Create(sizeof(RayCamera::Buffer), 0);
}

void RayCamera::OnUpdate()
{
	ProcessMovement();

	RayCamera::Buffer buffer{};
	buffer.Position = Vector4(m_Position, 0);
	buffer.Direction = Vector4(m_Direction, 0);
	buffer.Projection = m_InverseProjection;
	buffer.View = m_InverseView;
	buffer.RenderPass = uint32_t(m_RenderPass);

	m_CameraTransform->SetData(&buffer, sizeof(RayCamera::Buffer));
}

Ray RayCamera::GetRay() const
{
	return Ray(m_Position, m_Direction);
}

void RayCamera::OnResize(uint32_t width, uint32_t height)
{
	if (width == m_ViewportWidth && height == m_ViewportHeight) return;

	m_ViewportWidth = width;
	m_ViewportHeight = height;

	RecalculateProjection();
}

float RayCamera::GetRotationSpeed()
{
	return 0.3f;
}

void RayCamera::ProcessMovement()
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
	if (Input::IsKeyPressed(KeyCode::LeftShift))
		speed = 100.0f;
	else
		speed = 5.0f;

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
	}
}

void RayCamera::RecalculateProjection()
{
	m_Projection = glm::perspective(Mathf::ToRadians(m_Specs.VFov), (float)m_ViewportWidth / (float)m_ViewportHeight, 
		m_Specs.NearClip, m_Specs.FarClip);
	m_InverseProjection = glm::inverse(m_Projection);
}

void RayCamera::RecalculateView()
{
	m_View = glm::lookAt(m_Position, m_Position + m_Direction, Vector3(0.0f, 1.0f, 0.0f));
	m_InverseView = glm::inverse(m_View);
}
