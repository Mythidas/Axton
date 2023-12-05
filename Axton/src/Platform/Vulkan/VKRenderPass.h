#pragma once

#include "Axton/Renderer/RenderPass.h"
#include "VKObject.h"

#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKRenderPass : public RenderPass, public VKObject
	{
	public:
		VKRenderPass(const Specs& specs);

		virtual void Destroy();

		vk::RenderPass& GetRenderPass() { return m_RenderPass; }

		operator vk::RenderPass() const
		{
			return m_RenderPass;
		}

	private:
		vk::RenderPass m_RenderPass;
	};
}