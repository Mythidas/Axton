#include "axpch.h"
#include "VKRenderEngine.h"
#include "Axton/Event/Events.h"

#include <GLFW/glfw3.h>

namespace Axton::Vulkan
{
	VKRenderEngine* VKRenderEngine::s_Singleton = nullptr;

	VKRenderEngine::VKRenderEngine(void* windowHandle)
	{
		AX_ASSERT_CORE(!s_Singleton, "Only one VKRendererAPI can be created!");
		s_Singleton = this;

		m_GraphicsContext = VKGraphicsContext::Create(windowHandle, { VK_KHR_SWAPCHAIN_EXTENSION_NAME }, { "VK_LAYER_KHRONOS_validation" });
		m_Swapchain = VKSwapchain::Create();
		m_RenderPass = VKRenderPass::Create(m_Swapchain->GetFormat());

		m_Swapchain->CreateFramebuffers(m_RenderPass->GetRenderPass());

		vk::SemaphoreCreateInfo semaphoreInfo{};
		vk::FenceCreateInfo fenceInfo{};
		fenceInfo.setFlags(vk::FenceCreateFlagBits::eSignaled);

		m_ImageAvailable.resize(MAX_FRAMES_IN_FLIGHT);
		m_RenderFinished.resize(MAX_FRAMES_IN_FLIGHT);
		m_InFlight.resize(MAX_FRAMES_IN_FLIGHT);

		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_ImageAvailable[i] = m_GraphicsContext->GetDevice().createSemaphore(semaphoreInfo);
			m_RenderFinished[i] = m_GraphicsContext->GetDevice().createSemaphore(semaphoreInfo);
			m_InFlight[i] = m_GraphicsContext->GetDevice().createFence(fenceInfo);

			AX_ASSERT_CORE(m_ImageAvailable[i] && m_RenderFinished[i] && m_InFlight[i], "Failed to create SyncObjects!");
		}

		m_GraphicsContext->QueueDeletion([this]()
		{
			for (auto& semaphore : m_ImageAvailable)
			{
				m_GraphicsContext->GetDevice().destroy(semaphore);
			}
			for (auto& semaphore : m_RenderFinished)
			{
				m_GraphicsContext->GetDevice().destroy(semaphore);
			}
			for (auto& fence : m_InFlight)
			{
				m_GraphicsContext->GetDevice().destroy(fence);
			}
		});

		Events::OnWindowResize += AX_BIND_FNC(onWindowResized);
	}

	VKRenderEngine::~VKRenderEngine()
	{
		m_GraphicsContext->Destroy();
	}

	void VKRenderEngine::BeginFrame()
	{
		if (m_FramebufferInvalid) return;

		uint32_t currentFrame = m_GraphicsContext->GetCurrentFrame();

		if (m_GraphicsContext->GetDevice().waitForFences({ m_InFlight[currentFrame] }, VK_TRUE, UINT64_MAX) != vk::Result::eSuccess)
			return;

		if (m_FramebufferResized)
		{
			m_FramebufferResized = false;
			m_Swapchain->Recreate(m_RenderPass->GetRenderPass());
			return;
		}

		vk::Result result = m_GraphicsContext->GetDevice().acquireNextImageKHR(m_Swapchain->GetSwapchain(), UINT64_MAX, m_ImageAvailable[currentFrame], VK_NULL_HANDLE, &m_Swapchain->m_ImageIndex);

		if (result == vk::Result::eErrorOutOfDateKHR)
		{
			m_Swapchain->Recreate(m_RenderPass->GetRenderPass());
			return;
		}
		else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
		{
			AX_ASSERT_CORE(false, "Failed to aquire Swapchain Image!");
		}

		m_GraphicsContext->GetDevice().resetFences({ m_InFlight[currentFrame] });

		vk::CommandBufferBeginInfo beginInfo{};
		beginInfo
			.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit)
			.setPInheritanceInfo(nullptr);

		vk::CommandBuffer buffer = m_GraphicsContext->GetCommandBuffer();
		buffer.begin(beginInfo);

		vk::ClearValue clearColor(vk::ClearColorValue({ 0.0f, 0.0f, 0.0f, 1.0f }));
		vk::RenderPassBeginInfo renderPassInfo{};
		renderPassInfo
			.setRenderPass(m_RenderPass->GetRenderPass())
			.setFramebuffer(m_Swapchain->GetFramebuffer())
			.setRenderArea(vk::Rect2D().setOffset({ 0, 0 }).setExtent(m_Swapchain->GetExtent()))
			.setClearValueCount(1)
			.setPClearValues(&clearColor);

		buffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
	}

	void VKRenderEngine::EndFrame()
	{
		m_GraphicsContext->GetCommandBuffer().endRenderPass();
		m_GraphicsContext->GetCommandBuffer().end();

		uint32_t currentFrame = m_GraphicsContext->GetCurrentFrame();

		VKGraphicsContext::QueueSubmitInfo submitInfo{};
		submitInfo.WaitSemaphores = { m_ImageAvailable[currentFrame] };
		submitInfo.WaitStages = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
		submitInfo.SignalSemaphores = { m_RenderFinished[currentFrame] };
		submitInfo.Fence = m_InFlight[currentFrame];
		m_GraphicsContext->SubmitGraphicsQueue(submitInfo);

		std::vector<vk::SwapchainKHR> swapchains = { m_Swapchain->GetSwapchain() };
		submitInfo.WaitSemaphores = { m_RenderFinished[currentFrame] };
		vk::Result result = m_GraphicsContext->SubmitPresentQueue(submitInfo, swapchains, m_Swapchain->m_ImageIndex);

		if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || m_FramebufferResized)
		{
			m_FramebufferResized = false;
			m_Swapchain->Recreate(m_RenderPass->GetRenderPass());
			return;
		}
		else if (result != vk::Result::eSuccess)
		{
			AX_ASSERT_CORE(false, "Failed to present Swapchain Image!");
		}

		m_GraphicsContext->Update();
	}

	void VKRenderEngine::onWindowResized(int width, int height)
	{
		m_FramebufferInvalid = false;

		if (width == 0 || height == 0)
		{
			m_FramebufferInvalid = true;
		}

		m_FramebufferResized = true;
	}
}