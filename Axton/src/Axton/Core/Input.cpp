#include "axpch.h"
#include "Input.h"
#include "Application.h"
#include "Window.h"

#include <GLFW/glfw3.h>

namespace Axton
{
	Input* Input::s_Instance = nullptr;
	bool Input::s_Keys[352];
	Vector2 Input::s_MousePosition;

	Input::Input()
	{
		Window::OnKeyPressed += AX_BIND_FNC(Input::onKeyPressed);
		Window::OnKeyReleased += AX_BIND_FNC(Input::onKeyReleased);
		Window::OnMouseMoved += AX_BIND_FNC(Input::onMouseMoved);
		Window::OnMouseButtonPressed += AX_BIND_FNC(Input::onMouseButtonPressed);
		Window::OnMouseButtonReleased += AX_BIND_FNC(Input::onMouseButtonReleased);
	}

	void Input::Construct()
	{
		s_Instance = new Input();
	}

	bool Input::IsKeyPressed(Key key)
	{
		return s_Keys[key];
	}

	bool Input::IsKeyReleased(Key key)
	{
		return !s_Keys[key];
	}

	Vector2 Input::GetMousePosition()
	{
		return s_MousePosition;
	}

	void Input::LockCursor(bool lock)
	{
		Application::Get().GetWindow()->SetCursorMode(lock);
	}

	bool Input::onKeyPressed(int key)
	{
		s_Keys[key] = true;
		return false;
	}

	bool Input::onKeyReleased(int key)
	{
		s_Keys[key] = false;
		return false;
	}

	bool Input::onMouseMoved(double x, double y)
	{
		s_MousePosition = Vector2((float)x, (float)y);
		return false;
	}

	bool Input::onMouseButtonPressed(int button)
	{
		if (button == 0)
			s_Keys[KeyCode::LeftMouseButton] = true;
		else if (button == 1)
			s_Keys[KeyCode::RightMouseButton] = true;
		else if (button == 2)
			s_Keys[KeyCode::MiddleMouseButton] = true;

		return false;
	}

	bool Input::onMouseButtonReleased(int button)
	{
		if (button == 0)
			s_Keys[KeyCode::LeftMouseButton] = false;
		else if (button == 1)
			s_Keys[KeyCode::RightMouseButton] = false;
		else if (button == 2)
			s_Keys[KeyCode::MiddleMouseButton] = false;

		return false;
	}
}