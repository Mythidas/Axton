#include "axpch.h"
#include "VKSwapchain.h"
#include "Axton/Core/Application.h"

namespace Axton::Vulkan
{
	namespace Utils
	{
		vk::SurfaceFormatKHR chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> availableFormats)
		{
			for (const auto& format : availableFormats)
			{
				if (format.format == vk::Format::eB8G8R8A8Snorm && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
				{
					return format;
				}
			}

			return availableFormats[0];
		}

		vk::PresentModeKHR choosePresentMode(const std::vector<vk::PresentModeKHR> availableModes)
		{
			for (const auto& mode : availableModes)
			{
				if (mode == vk::PresentModeKHR::eMailbox)
					return mode;
			}

			return vk::PresentModeKHR::eFifo;
		}

		vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities)
		{
			if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
			{
				return capabilities.currentExtent;
			}
			else
			{
				vk::Extent2D actualExtent =
				{
					Application::Get().GetWindow().GetWidth(),
					Application::Get().GetWindow().GetHeight()
				};

				actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
				actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

				return actualExtent;
			}
		}
	}

	Ref<VKSwapchain> VKSwapchain::Create(Ref<VKGraphicsContext> graphicsContext)
	{
		Ref<VKSwapchain> swapchain = CreateRef<VKSwapchain>();
		swapchain->m_GraphicsContext = graphicsContext;
		swapchain->createSwapchain();

		graphicsContext->QueueDeletion([swapchain]()
		{
			for (auto& image : swapchain->m_Images)
			{
				image->Destroy();
			}

			for (auto& buffer : swapchain->m_Framebuffers)
			{
				swapchain->m_GraphicsContext->GetDevice().destroy(buffer);
			}

			swapchain->m_GraphicsContext->GetDevice().destroy(swapchain->m_Swapchain);

			swapchain->m_Images.clear();
			swapchain->m_Framebuffers.clear();
		});

		return swapchain;
	}

	void VKSwapchain::CreateFramebuffers(vk::RenderPass renderPass)
	{
		m_Framebuffers.resize(m_Images.size());

		for (size_t i = 0; i < m_Framebuffers.size(); i++)
		{
			vk::ImageView attachments[] = { m_Images[i]->GetView() };

			vk::FramebufferCreateInfo createInfo{};
			createInfo
				.setRenderPass(renderPass)
				.setAttachmentCount(1)
				.setPAttachments(attachments)
				.setWidth(m_SwapchainExtent.width)
				.setHeight(m_SwapchainExtent.height)
				.setLayers(1);

			m_Framebuffers[i] = m_GraphicsContext->GetDevice().createFramebuffer(createInfo);
		}
	}

	void VKSwapchain::Recreate(vk::RenderPass renderPass)
	{
		m_GraphicsContext->GetDevice().waitIdle();

		for (auto& image : m_Images)
		{
			image->Destroy();
		}

		for (auto& buffer : m_Framebuffers)
		{
			m_GraphicsContext->GetDevice().destroy(buffer);
		}

		m_GraphicsContext->GetDevice().destroy(m_Swapchain);

		m_Images.clear();
		m_Framebuffers.clear();
		
		createSwapchain();
		CreateFramebuffers(renderPass);
	}

	void VKSwapchain::createSwapchain()
	{
		vk::SurfaceCapabilitiesKHR capabilities = m_GraphicsContext->GetPhysicalDevice().getSurfaceCapabilitiesKHR(m_GraphicsContext->GetSurface());
		std::vector<vk::SurfaceFormatKHR> surfaceFormats = m_GraphicsContext->GetPhysicalDevice().getSurfaceFormatsKHR(m_GraphicsContext->GetSurface());
		std::vector<vk::PresentModeKHR> presentModes = m_GraphicsContext->GetPhysicalDevice().getSurfacePresentModesKHR(m_GraphicsContext->GetSurface());

		vk::SurfaceFormatKHR format = Utils::chooseSurfaceFormat(surfaceFormats);
		vk::PresentModeKHR mode = Utils::choosePresentMode(presentModes);
		vk::Extent2D extent = Utils::chooseSwapExtent(capabilities);

		uint32_t imageCount = capabilities.minImageCount + 1;
		if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
		{
			imageCount = capabilities.maxImageCount;
		}

		vk::SwapchainCreateInfoKHR createInfo{};
		createInfo
			.setSurface(m_GraphicsContext->GetSurface())
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
		for (const uint32_t& family : m_GraphicsContext->GetQueueFamilyIndices())
		{
			families.insert(family);
		}

		if (families.size() > 1)
		{
			createInfo
				.setImageSharingMode(vk::SharingMode::eConcurrent)
				.setQueueFamilyIndexCount(static_cast<uint32_t>(m_GraphicsContext->GetQueueFamilyIndices().size()))
				.setPQueueFamilyIndices(m_GraphicsContext->GetQueueFamilyIndices().data());
		}
		else
		{
			createInfo
				.setImageSharingMode(vk::SharingMode::eExclusive)
				.setQueueFamilyIndexCount(0)
				.setPQueueFamilyIndices(nullptr);
		}

		m_Swapchain = m_GraphicsContext->GetDevice().createSwapchainKHR(createInfo);
		AX_ASSERT_CORE(m_Swapchain, "Failed to create Swapchain!");

		for (const auto& image : m_GraphicsContext->GetDevice().getSwapchainImagesKHR(m_Swapchain))
		{
			m_Images.push_back(VKImage::Specs().setViewType(vk::ImageViewType::e2D).setFormat(format.format).setAspectFlags(vk::ImageAspectFlagBits::eColor).Build(image));
		}
		m_SwapchainFormat = format.format;
		m_SwapchainExtent = extent;
	}
}