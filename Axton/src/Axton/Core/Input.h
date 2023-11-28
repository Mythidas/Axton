#pragma once

#include "Defines.h"
#include "KeyCodes.h"
#include "Axton/Math/Vector.h"

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

		static bool onKeyPressed(int key);
		static bool onKeyReleased(int key);
		static bool onMouseMoved(double x, double y);
		static bool onMouseButtonPressed(int button);
		static bool onMouseButtonReleased(int button);
	};
}