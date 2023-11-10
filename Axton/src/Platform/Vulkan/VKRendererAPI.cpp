#include "axpch.h"
#include "VKRendererAPI.h"
#include "Axton/Event/Events.h"

#include <GLFW/glfw3.h>

namespace Axton::Vulkan
{
	VKRendererAPI* VKRendererAPI::s_Singleton = nullptr;

	VKRendererAPI::VKRendererAPI(void* windowHandle)
	{
		AX_ASSERT_CORE(!s_Singleton, "Only one VKRendererAPI can be created!");
		s_Singleton = this;

		m_GraphicsContext = VKGraphicsContext::Create(windowHandle, { VK_KHR_SWAPCHAIN_EXTENSION_NAME }, { "VK_LAYER_KHRONOS_validation" });
		m_Swapchain = VKSwapchain::Create(m_GraphicsContext);
		m_GraphicsPipeline = VKGraphicsPipeline::Create(m_GraphicsContext, m_Swapchain);

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

		Events::OnWindowResize += AX_BIND_FNC(OnWindowResized);
	}

	VKRendererAPI::~VKRendererAPI()
	{
		m_GraphicsContext->GetDevice().waitIdle();
		m_GraphicsContext->Destroy();
	}

	void VKRendererAPI::BeginFrame()
	{
		if (m_FramebufferInvalid) return;

		uint32_t currentFrame = m_GraphicsContext->GetCurrentFrame();

		if (m_GraphicsContext->GetDevice().waitForFences({ m_InFlight[currentFrame] }, VK_TRUE, UINT64_MAX) != vk::Result::eSuccess)
			return;

		if (m_FramebufferResized)
		{
			m_FramebufferResized = false;
			m_Swapchain->Recreate(m_GraphicsPipeline->GetRenderPass());
			return;
		}

		vk::Result result = m_GraphicsContext->GetDevice().acquireNextImageKHR(m_Swapchain->GetSwapchain(), UINT64_MAX, m_ImageAvailable[currentFrame], VK_NULL_HANDLE, &m_Swapchain->m_ImageIndex);

		if (result == vk::Result::eErrorOutOfDateKHR)
		{
			m_Swapchain->Recreate(m_GraphicsPipeline->GetRenderPass());
			return;
		}
		else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
		{
			AX_ASSERT_CORE(false, "Failed to aquire Swapchain Image!");
		}

		m_GraphicsContext->GetDevice().resetFences({ m_InFlight[currentFrame] });

		m_GraphicsPipeline->BeginRender();
	}

	void VKRendererAPI::EndFrame()
	{
		m_GraphicsPipeline->EndRender();
		m_GraphicsContext->FlushCommandQueue();

		uint32_t currentFrame = m_GraphicsContext->GetCurrentFrame();

		VKGraphicsContext::QueueSubmitInfo submitInfo{};
		submitInfo.WaitSemaphores = { m_ImageAvailable[currentFrame] };
		submitInfo.WaitStages = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
		submitInfo.SignalSemaphores = { m_RenderFinished[currentFrame] };
		submitInfo.Fence = m_InFlight[currentFrame];
		m_GraphicsContext->SubmitQueue(submitInfo);

		std::vector<vk::SwapchainKHR> swapchains = { m_Swapchain->GetSwapchain() };
		submitInfo.WaitSemaphores = { m_RenderFinished[currentFrame] };
		vk::Result result = m_GraphicsContext->PresentQueue(submitInfo, swapchains, m_Swapchain->m_ImageIndex);

		if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || m_FramebufferResized)
		{
			m_FramebufferResized = false;
			m_Swapchain->Recreate(m_GraphicsPipeline->GetRenderPass());
			return;
		}
		else if (result != vk::Result::eSuccess)
		{
			AX_ASSERT_CORE(false, "Failed to present Swapchain Image!");
		}

		m_GraphicsContext->Update();
	}

	void VKRendererAPI::OnUpdate()
	{

	}

	void VKRendererAPI::OnWindowResized(int width, int height)
	{
		m_FramebufferInvalid = false;

		if (width == 0 || height == 0)
		{
			m_FramebufferInvalid = true;
		}

		m_FramebufferResized = true;
	}
}