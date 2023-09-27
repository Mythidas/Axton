#include "axpch.h"
#include "Input.h"
#include "Application.h"
#include "Axton/Event/Events.h"
#include "Axton/Math/Math.h"

#include <GLFW/glfw3.h>

namespace Axton
{
	Input* Input::s_Instance = nullptr;
	bool Input::s_Keys[352];
	Vector2 Input::s_MousePosition;

	Input::Input()
	{
		Events::OnKeyPressed += AX_BIND_FNC(Input::OnKeyPressed);
		Events::OnKeyReleased += AX_BIND_FNC(Input::OnKeyReleased);
		Events::OnMouseMoved += AX_BIND_FNC(Input::OnMouseMoved);
		Events::OnMouseButtonPressed += AX_BIND_FNC(Input::OnMouseButtonPressed);
		Events::OnMouseButtonReleased += AX_BIND_FNC(Input::OnMouseButtonReleased);
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
		Application::Get().GetWindow().SetCursorMode(lock);
	}

	void Input::OnKeyPressed(int key)
	{
		s_Keys[key] = true;
	}

	void Input::OnKeyReleased(int key)
	{
		s_Keys[key] = false;
	}

	void Input::OnMouseMoved(double x, double y)
	{
		s_MousePosition = Vector2((float)x, (float)y);
	}

	void Input::OnMouseButtonPressed(int button)
	{
		if (button == 0)
			s_Keys[KeyCode::LeftMouseButton] = true;
		else if (button == 1)
			s_Keys[KeyCode::RightMouseButton] = true;
		else if (button == 2)
			s_Keys[KeyCode::MiddleMouseButton] = true;
	}

	void Input::OnMouseButtonReleased(int button)
	{
		if (button == 0)
			s_Keys[KeyCode::LeftMouseButton] = false;
		else if (button == 1)
			s_Keys[KeyCode::RightMouseButton] = false;
		else if (button == 2)
			s_Keys[KeyCode::MiddleMouseButton] = false;
	}
}