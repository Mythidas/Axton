#pragma once

#include "Axton.h"

using namespace Axton;

class FlyCamera : public NativeScriptComponent
{
public:
	virtual void OnUpdate() override
	{
		Transform* transform = World::GetComponent<Transform>(Parent);

		if (Input::IsKeyPressed(KeyCode::LeftMouseButton))
		{
			Input::LockCursor(true);

			float speed = 5.0f * Time::GetDeltaTime();

			if (Input::IsKeyPressed(KeyCode::W))
			{
				transform->Position += transform->Forward * speed;
			}
			if (Input::IsKeyPressed(KeyCode::S))
			{
				transform->Position -= transform->Forward * speed;
			}
			if (Input::IsKeyPressed(KeyCode::A))
			{
				transform->Position -= transform->GetRight() * speed;
			}
			if (Input::IsKeyPressed(KeyCode::D))
			{
				transform->Position += transform->GetRight() * speed;
			}

			Vector2 mouseDelta = { Input::GetMousePosition().x - m_LastMousePosition.x, m_LastMousePosition.y - Input::GetMousePosition().y };
			m_LastMousePosition = Input::GetMousePosition();

			float sensitivity = 4.0f;
			mouseDelta *= speed;

			m_Yaw += mouseDelta.x;
			m_Pitch += mouseDelta.y;

			if (m_Pitch > 89.0f)
				m_Pitch = 89.0f;
			if (m_Pitch < -89.0f)
				m_Pitch = -89.0f;
		}
		else
		{
			Input::LockCursor(false);
			m_LastMousePosition = Input::GetMousePosition();
		}

		//Vector3 direction;
		//direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		//direction.y = sin(glm::radians(m_Pitch));
		//direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

		//transform->Forward = glm::normalize(direction);

	}

private:
	Vector2 m_LastMousePosition{ 0.0f, 0.0f };
	float m_Pitch = 0.0f, m_Yaw = -90.0f;
};