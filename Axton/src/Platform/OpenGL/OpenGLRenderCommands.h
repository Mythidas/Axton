#pragma once

#include "Axton/Renderer/RenderCommands.h"

namespace Axton
{
	class OpenGLRenderCommands
	{
	public:
		static void Clear();
		static void SetClearColor(const Vector4& color);
		static void SetBlendMode(bool enabled);
		static void SetViewport(int& width, int& height);

		static void DrawIndexed(uint32_t count);
	};
}