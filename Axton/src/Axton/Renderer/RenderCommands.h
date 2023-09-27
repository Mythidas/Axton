#pragma once

#include "Axton/Math/Math.h"
#include "RendererAPI.h"

namespace Axton
{
	class RenderCommands
	{
	public:
		static void ClearScreen()
		{
			s_API->ClearScreen();
		}

		static void SetClearColor(const Vector4& clearColor)
		{
			s_API->SetClearColor(clearColor);
		}

		static void SetBlendMode(bool enabled)
		{
			s_API->SetBlendMode(enabled);
		}

		static void SetViewport(int& width, int& height)
		{
			s_API->SetViewport(width, height);
		}

		static void EnableDepthTest(bool enabled)
		{
			s_API->EnableDepthTest(enabled);
		}

		static void DrawIndexed(uint32_t count)
		{
			s_API->DrawIndexed(count);
		}

	private:
		static Scope<RendererAPI> s_API;
	};
}