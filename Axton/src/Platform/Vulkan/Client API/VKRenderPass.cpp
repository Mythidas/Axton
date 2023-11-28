#include "axpch.h"
#include "VKRenderPass.h"
#include "../VKUtils.h"
#include "../VKRenderEngine.h"


namespace Axton::Vulkan
{
	VKRenderPass::VKRenderPass(const Specs& specs)
	{
		auto availableFormats = VKRenderEngine::GetPhysicalDevice().getSurfaceFormatsKHR(VKRenderEngine::GetSurface());
		vk::SurfaceFormatKHR format = VKUtils::ChooseSurfaceFormat(availableFormats);

		vk::AttachmentDescription colorAttachment{};
		colorAttachment
			.setFormat(format.format)
			.setSamples(vk::SampleCountFlagBits::e1)
			.setLoadOp(vk::AttachmentLoadOp::eClear)
			.setStoreOp(vk::AttachmentStoreOp::eStore)
			.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
			.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
			.setInitialLayout(vk::ImageLayout::eUndefined)
			.setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

		vk::AttachmentReference colorAttachmentRef{};
		colorAttachmentRef
			.setAttachment(0)
			.setLayout(vk::ImageLayout::eColorAttachmentOptimal);

		vk::SubpassDependency dependency{};
		dependency
			.setSrcSubpass(VK_SUBPASS_EXTERNAL)
			.setDstSubpass(0)
			.setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
			.setSrcAccessMask(vk::AccessFlagBits::eNone)
			.setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
			.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite);

		vk::SubpassDescription subpass{};
		subpass
			.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
			.setColorAttachmentCount(1)
			.setPColorAttachments(&colorAttachmentRef);

		vk::RenderPassCreateInfo renderPassInfo{};
		renderPassInfo
			.setAttachmentCount(1)
			.setPAttachments(&colorAttachment)
			.setSubpassCount(1)
			.setPSubpasses(&subpass)
			.setDependencyCount(1)
			.setPDependencies(&dependency);

		vk::Device device = VKRenderEngine::GetDevice();
		m_RenderPass = device.createRenderPass(renderPassInfo);
		AssertCore(m_RenderPass, "Failed to create RenderPass!");
	}

	VKRenderPass::~VKRenderPass()
	{
		VKRenderEngine::GetDevice().destroy(m_RenderPass);
	}
}