#pragma once
#include "VKGraphicsContext.h"
#include "VKSwapchain.h"
#include "VKGraphicsPipeline.h"
#include "Axton/Renderer/RendererAPI.h"

#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKRendererAPI : public RendererAPI
	{
	public:
		VKRendererAPI(void* windowHandle);
		~VKRendererAPI();

		virtual void BeginFrame() override;
		virtual void EndFrame() override;
		virtual void OnUpdate() override;

		static VKGraphicsContext& GetGraphicsContext() { return *s_Singleton->m_GraphicsContext; }
		static VKSwapchain& GetSwapchain() { return *s_Singleton->m_Swapchain; }
		static VKGraphicsPipeline& GetGraphicsPipeline() { return *s_Singleton->m_GraphicsPipeline; }

	private:
		void OnWindowResized(int width, int height);

	private:
		static VKRendererAPI* s_Singleton;

		Ref<VKGraphicsContext> m_GraphicsContext;
		Ref<VKSwapchain> m_Swapchain;
		Ref<VKGraphicsPipeline> m_GraphicsPipeline;

		std::vector<vk::Semaphore> m_ImageAvailable;
		std::vector<vk::Semaphore> m_RenderFinished;
		std::vector<vk::Fence> m_InFlight;

		bool m_FramebufferInvalid = false;
		bool m_FramebufferResized = false;
	};
}