#pragma once

#include "Axton/Math/Math.h"

namespace Axton
{
	class RenderCommands
	{
	public:
		static void ClearScreen();
		static void SetClearColor(const Vector4& clearColor);
		static void SetBlendMode(bool enabled);
		static void SetViewport(int& width, int& height);

		static void DrawIndexed(uint32_t count);
	};
}