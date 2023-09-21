#pragma once

#include "Defines.h"
#include "KeyCodes.h"

namespace Axton
{
	class AX_API Input
	{
		Input();
	public:
		static void Construct();

		static bool IsKeyPressed(KeyCode key);
		static bool IsKeyReleased(KeyCode key);

	private:
		static Input* s_Instance;
		static bool s_Keys[1024];

		static void OnKeyPressed(int key);
		static void OnKeyReleased(int key);
	};
}