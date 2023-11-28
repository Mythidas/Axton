#pragma once

#include "Axton/Renderer/RenderPass.h"

#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKRenderPass : public RenderPass
	{
	public:
		VKRenderPass(const Specs& specs);
		~VKRenderPass();

		operator vk::RenderPass() const
		{
			return m_RenderPass;
		}

	private:
		vk::RenderPass m_RenderPass;
	};
}