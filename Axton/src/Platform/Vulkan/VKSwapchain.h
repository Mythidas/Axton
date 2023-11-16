#pragma once

#include "VKImageCore.h"
#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKSwapchain
	{
		friend class VKRenderEngine;
	public:
		static Ref<VKSwapchain> Create();

		void CreateFramebuffers(vk::RenderPass renderPass);
		void Recreate(vk::RenderPass renderPass);

		vk::SwapchainKHR& GetSwapchain() { return m_Swapchain; }
		vk::Format& GetFormat() { return m_SwapchainFormat; }
		vk::Extent2D& GetExtent() { return m_SwapchainExtent; }
		const std::vector<Ref<VKImageCore>>& GetImages() { return m_Images; }
		const std::vector<vk::Framebuffer>& GetFramebuffers() { return m_Framebuffers; }
		vk::Framebuffer& GetFramebuffer() { return m_Framebuffers[m_ImageIndex]; }


	private:
		void createSwapchain();

	private:
		vk::SwapchainKHR m_Swapchain;

		std::vector<Ref<VKImageCore>> m_Images;
		vk::Format m_SwapchainFormat;
		vk::Extent2D m_SwapchainExtent;
		std::vector<vk::Framebuffer> m_Framebuffers;

		uint32_t m_ImageIndex = 0;
	};
}