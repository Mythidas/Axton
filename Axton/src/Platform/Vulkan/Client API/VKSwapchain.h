#pragma once

#include "Axton/Renderer/Swapchain.h"
#include "../VKImage.h"

namespace Axton::Vulkan
{
	class VKSwapchain : public Swapchain
	{
		friend class VKRenderEngine;

	public:
		VKSwapchain(const Specs& specs);
		~VKSwapchain();

		virtual void Recreate() override;

		virtual Extent2D GetExtent() const override { return Extent2D(m_Extent.width, m_Extent.height); }
		virtual Ref<RenderPass> GetRenderPass() const override { return m_Specs.pRenderPass; };

		operator vk::SwapchainKHR() const
		{
			return m_Swapchain;
		}

	private:
		void createSwapchain();

	private:
		Specs m_Specs;
		vk::SwapchainKHR m_Swapchain;

		std::vector<Ref<VKImage>> m_Images;
		vk::Format m_Format;
		vk::Extent2D m_Extent;
		std::vector<vk::Framebuffer> m_Framebuffers;

		uint32_t m_ImageIndex = 0;
	};
}