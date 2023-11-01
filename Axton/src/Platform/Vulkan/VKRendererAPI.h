#pragma once

#include "Axton/Renderer/RendererAPI.h"

namespace Axton::Vulkan
{
	class VKRendererAPI : public Axton::RendererAPI
	{
	public:
		virtual void clearScreen() override;
		virtual void setClearColor(const Vector4& clearColor) override;
		virtual void setBlendMode(bool enabled) override;
		virtual void setViewport(int& width, int& height) override;
		virtual void enableDepthTest(bool enabled) override;

		virtual void drawIndexed(uint32_t count) override;
	};
}