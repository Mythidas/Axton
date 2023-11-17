#pragma once
#include "VKGraphicsContext.h"
#include "VKSwapchain.h"
#include "VKRenderPass.h"
#include "Axton/Renderer/RenderEngine.h"

#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKRenderEngine : public RenderEngine
	{
	public:
		VKRenderEngine(void* windowHandle, const Specs& specs);
		~VKRenderEngine();

		virtual void BeginFrame() override;
		virtual void EndFrame() override;
		virtual void RenderFrame() override;

		static Ref<VKGraphicsContext> GetGraphicsContext() { return s_Singleton->m_GraphicsContext; }
		static Ref<VKSwapchain> GetSwapchain() { return s_Singleton->m_Swapchain; }
		static Ref<VKRenderPass> GetRenderPass() { return s_Singleton->m_RenderPass; }

	private:
		void onWindowResized(int width, int height);

	private:
		static VKRenderEngine* s_Singleton;

		Specs m_Specs;
		Ref<VKGraphicsContext> m_GraphicsContext;
		Ref<VKSwapchain> m_Swapchain;
		Ref<VKRenderPass> m_RenderPass;

		std::vector<vk::Semaphore> m_ImageAvailable;
		std::vector<vk::Semaphore> m_RenderFinished;
		std::vector<vk::Fence> m_InFlight;

		bool m_FramebufferInvalid = false;
		bool m_FramebufferResized = false;
		bool m_FrameInvalid = false;
	};
}