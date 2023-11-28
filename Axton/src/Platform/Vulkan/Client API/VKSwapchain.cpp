#include "axpch.h"
#include "VKSwapchain.h"
#include "VKRenderPass.h"
#include "../VKUtils.h"
#include "../VKRenderEngine.h"

namespace Axton::Vulkan
{
	VKSwapchain::VKSwapchain(const Specs& specs)
		: m_Specs(specs)
	{
		createSwapchain();
	}

	VKSwapchain::~VKSwapchain()
	{
		VKRenderEngine::GetDevice().waitIdle();

		for (auto& fb : m_Framebuffers)
		{
			VKRenderEngine::GetDevice().destroy(fb);
		}

		VKRenderEngine::GetDevice().destroy(m_Swapchain);

		m_Images.clear();
	}

	void VKSwapchain::Recreate()
	{
		VKRenderEngine::GetDevice().waitIdle();

		for (auto& fb : m_Framebuffers)
		{
			VKRenderEngine::GetDevice().destroy(fb);
		}

		VKRenderEngine::GetDevice().destroy(m_Swapchain);

		m_Images.clear();
		m_Framebuffers.clear();

		createSwapchain();
	}

	void VKSwapchain::createSwapchain()
	{
		Ref<VKGraphicsContext> gContext = VKRenderEngine::GetGraphicsContext();

		vk::SurfaceCapabilitiesKHR capabilities = gContext->GetPhysicalDevice().getSurfaceCapabilitiesKHR(gContext->GetSurface());
		std::vector<vk::SurfaceFormatKHR> surfaceFormats = gContext->GetPhysicalDevice().getSurfaceFormatsKHR(gContext->GetSurface());
		std::vector<vk::PresentModeKHR> presentModes = gContext->GetPhysicalDevice().getSurfacePresentModesKHR(gContext->GetSurface());

		vk::SurfaceFormatKHR format = VKUtils::ChooseSurfaceFormat(surfaceFormats);
		vk::PresentModeKHR mode = VKUtils::ChoosePresentMode(presentModes);
		vk::Extent2D extent = VKUtils::ChooseSwapExtent(capabilities);

		uint32_t imageCount = capabilities.minImageCount + 1;
		if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
		{
			imageCount = capabilities.maxImageCount;
		}

		vk::SwapchainCreateInfoKHR createInfo{};
		createInfo
			.setSurface(gContext->GetSurface())
			.setMinImageCount(imageCount)
			.setImageFormat(format.format)
			.setImageColorSpace(format.colorSpace)
			.setImageExtent(extent)
			.setImageArrayLayers(1)
			.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
			.setPreTransform(capabilities.currentTransform)
			.setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
			.setPresentMode(mode)
			.setClipped(VK_TRUE)
			.setOldSwapchain(VK_NULL_HANDLE);

		std::unordered_set<uint32_t> families;
		for (const uint32_t& family : gContext->GetQueueFamilyIndices())
		{
			families.insert(family);
		}

		if (families.size() > 1)
		{
			createInfo
				.setImageSharingMode(vk::SharingMode::eConcurrent)
				.setQueueFamilyIndexCount(static_cast<uint32_t>(gContext->GetQueueFamilyIndices().size()))
				.setPQueueFamilyIndices(gContext->GetQueueFamilyIndices().data());
		}
		else
		{
			createInfo
				.setImageSharingMode(vk::SharingMode::eExclusive)
				.setQueueFamilyIndexCount(0)
				.setPQueueFamilyIndices(nullptr);
		}

		m_Swapchain = gContext->GetDevice().createSwapchainKHR(createInfo);
		AssertCore(m_Swapchain, "Failed to create Swapchain!");

		for (const auto& image : gContext->GetDevice().getSwapchainImagesKHR(m_Swapchain))
		{
			m_Images.push_back(VKImage::Specs().setViewType(vk::ImageViewType::e2D).setFormat(format.format).setAspectFlags(vk::ImageAspectFlagBits::eColor).Build(image));
		}
		m_Format = format.format;
		m_Extent = extent;

		m_Framebuffers.resize(m_Images.size());

		for (size_t i = 0; i < m_Framebuffers.size(); i++)
		{
			vk::ImageView attachments[] = { m_Images[i]->GetView() };

			VKRenderPass& vkRenderPass = static_cast<VKRenderPass&>(*m_Specs.pRenderPass);
			vk::FramebufferCreateInfo createInfo{};
			createInfo
				.setRenderPass(vkRenderPass)
				.setAttachmentCount(1)
				.setPAttachments(attachments)
				.setWidth(m_Extent.width)
				.setHeight(m_Extent.height)
				.setLayers(1);

			m_Framebuffers[i] = VKRenderEngine::GetDevice().createFramebuffer(createInfo);
		}
	}
}