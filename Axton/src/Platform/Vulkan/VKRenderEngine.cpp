#include "axpch.h"
#include "VKRenderEngine.h"
#include "Axton/Core/Window.h"
#include "Client API/VKSwapchain.h"
#include "Client API/VKRenderPass.h"

#include <GLFW/glfw3.h>

namespace Axton::Vulkan
{
	namespace Utils
	{
		std::vector<const char*> getRequiredExtensions(RenderExtension extensions)
		{
			std::vector<const char*> extensionNames;

			if (extensions & RenderExtension::Graphics)
			{
				extensionNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
			}
			if (extensions & RenderExtension::RayTracing)
			{
				extensionNames.push_back(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);
				extensionNames.push_back(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME);
				extensionNames.push_back(VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME);
				extensionNames.push_back(VK_KHR_SPIRV_1_4_EXTENSION_NAME);
			}

			return extensionNames;
		}
	}

	VKRenderEngine* VKRenderEngine::s_Singleton = nullptr;

	VKRenderEngine::VKRenderEngine(void* windowHandle, const Specs& specs)
		: m_Specs(specs)
	{
		AssertCore(!s_Singleton, "Only one VKRendererAPI can be created!");
		s_Singleton = this;

		m_GraphicsContext = VKGraphicsContext::Create(windowHandle, Utils::getRequiredExtensions(specs.Extensions), { "VK_LAYER_KHRONOS_validation" });

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

			AssertCore(m_ImageAvailable[i] && m_RenderFinished[i] && m_InFlight[i], "Failed to create SyncObjects!");
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

		Window::OnWindowResize += AX_BIND_FNC(onWindowResized);
	}

	VKRenderEngine::~VKRenderEngine()
	{
		m_GraphicsContext->Destroy();
	}

	void VKRenderEngine::RenderFrame()
	{
		VKSwapchain& vkSwapchain = static_cast<VKSwapchain&>(*GetSwapchain().get());
		VKRenderPass& vkRenderPass = static_cast<VKRenderPass&>(*GetRenderPass().get());

		if (m_FramebufferInvalid)
		{
			m_GraphicsContext->ClearGraphicsCommands();
			return;
		}

		uint32_t currentFrame = m_GraphicsContext->GetCurrentFrame();

		if (m_GraphicsContext->GetDevice().waitForFences({ m_InFlight[currentFrame] }, VK_TRUE, UINT64_MAX) != vk::Result::eSuccess) return;

		if (m_FramebufferResized)
		{
			m_FramebufferResized = false;
			GetSwapchain()->Recreate();
			m_GraphicsContext->ClearGraphicsCommands();
			return;
		}

		vk::Result result = m_GraphicsContext->GetDevice().acquireNextImageKHR(vkSwapchain, UINT64_MAX, m_ImageAvailable[currentFrame], VK_NULL_HANDLE, &vkSwapchain.m_ImageIndex);

		if (result == vk::Result::eErrorOutOfDateKHR)
		{
			GetSwapchain()->Recreate();
			m_GraphicsContext->ClearGraphicsCommands();
			return;
		}
		else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
		{
			AssertCore(false, "Failed to aquire Swapchain Image!");
			m_GraphicsContext->ClearGraphicsCommands();
			return;
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
			.setRenderPass(vkRenderPass)
			.setFramebuffer(vkSwapchain.m_Framebuffers[vkSwapchain.m_ImageIndex])
			.setRenderArea(vk::Rect2D().setOffset({ 0, 0 }).setExtent(vkSwapchain.m_Extent))
			.setClearValueCount(1)
			.setPClearValues(&clearColor);

		m_GraphicsContext->GetCommandBuffer().beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
		m_GraphicsContext->FlushGraphicsCommands();
		m_GraphicsContext->GetCommandBuffer().endRenderPass();

		m_GraphicsContext->FlushComputeCommands();

		m_GraphicsContext->GetCommandBuffer().end();

		VKGraphicsContext::QueueSubmitInfo submitInfo{};
		submitInfo.WaitSemaphores = { m_ImageAvailable[currentFrame] };
		submitInfo.WaitStages = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
		submitInfo.SignalSemaphores = { m_RenderFinished[currentFrame] };
		submitInfo.Fence = m_InFlight[currentFrame];
		m_GraphicsContext->SubmitGraphicsQueue(submitInfo);

		std::vector<vk::SwapchainKHR> swapchains = { vkSwapchain };
		submitInfo.WaitSemaphores = { m_RenderFinished[currentFrame] };
		result = m_GraphicsContext->SubmitPresentQueue(submitInfo, swapchains, vkSwapchain.m_ImageIndex);

		if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || m_FramebufferResized)
		{
			m_FramebufferResized = false;
			GetSwapchain()->Recreate();
			return;
		}
		else if (result != vk::Result::eSuccess)
		{
			AssertCore(false, "Failed to present Swapchain Image!");
		}

		m_GraphicsContext->Update();
	}

	bool VKRenderEngine::onWindowResized(int width, int height)
	{
		m_FramebufferInvalid = false;

		if (width == 0 || height == 0)
		{
			m_FramebufferInvalid = true;
		}

		m_FramebufferResized = true;
		return false;
	}
}