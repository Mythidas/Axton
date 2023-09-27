#pragma once

#include "Defines.h"
#include "KeyCodes.h"
#include "Axton/Math/Math.h"

namespace Axton
{
	class Input
	{
		Input();
	public:
		static void Construct();

		static bool IsKeyPressed(Key key);
		static bool IsKeyReleased(Key key);
		static Vector2 GetMousePosition();

		static void LockCursor(bool lock);

	private:
		static Input* s_Instance;
		static bool s_Keys[352];
		static Vector2 s_MousePosition;

		static void OnKeyPressed(int key);
		static void OnKeyReleased(int key);
		static void OnMouseMoved(double x, double y);
		static void OnMouseButtonPressed(int button);
		static void OnMouseButtonReleased(int button);
	};
}