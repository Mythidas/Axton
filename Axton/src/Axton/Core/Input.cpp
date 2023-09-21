#include "axpch.h"
#include "Input.h"
#include "Axton/Event/Events.h"

#include <GLFW/glfw3.h>

namespace Axton
{
	Input* Input::s_Instance = nullptr;
	bool Input::s_Keys[1024];

	Input::Input()
	{
		Events::OnKeyPressed += AX_BIND_FNC(Input::OnKeyPressed);
		Events::OnKeyReleased += AX_BIND_FNC(Input::OnKeyReleased);
	}

	void Input::Construct()
	{
		s_Instance = new Input();
	}

	bool Input::IsKeyPressed(KeyCode key)
	{
		return s_Keys[key];
	}

	bool Input::IsKeyReleased(KeyCode key)
	{
		return !s_Keys[key];
	}

	void Input::OnKeyPressed(int key)
	{
		s_Keys[key] = true;
	}

	void Input::OnKeyReleased(int key)
	{
		s_Keys[key] = false;
	}
}