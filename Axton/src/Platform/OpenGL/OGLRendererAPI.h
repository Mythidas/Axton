#pragma once

#include "Axton/Renderer/RendererAPI.h"

namespace Axton::OpenGL
{
	class OGLRendererAPI : public Axton::RendererAPI
	{
	public:
		virtual void ClearScreen() override;
		virtual void SetClearColor(const Vector4& clearColor) override;
		virtual void SetBlendMode(bool enabled) override;
		virtual void SetViewport(int& width, int& height) override;
		virtual void EnableDepthTest(bool enabled) override;

		virtual void DrawIndexed(uint32_t count) override;
	};
}