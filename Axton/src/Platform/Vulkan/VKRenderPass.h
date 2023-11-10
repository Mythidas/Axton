#pragma once

#include "VKGraphicsContext.h"
#include "Axton/Core/Defines.h"

#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKRenderPass
	{
	public:
		static Ref<VKRenderPass> Create(Ref<VKGraphicsContext> graphicsContext, vk::Format format);

		vk::RenderPass& GetRenderPass() { return m_RenderPass; }

	private:
		vk::RenderPass m_RenderPass;
	};
}