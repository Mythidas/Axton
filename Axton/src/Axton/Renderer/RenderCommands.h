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

		static void DrawIndexed(uint32_t count);
	};
}