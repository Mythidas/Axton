#pragma once
#include "VKGraphicsContext.h"
#include "VKSwapchain.h"
#include "VKRenderPass.h"
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

		static Ref<VKGraphicsContext> GetGraphicsContext() { return s_Singleton->m_GraphicsContext; }
		static Ref<VKSwapchain> GetSwapchain() { return s_Singleton->m_Swapchain; }
		static Ref<VKRenderPass> GetRenderPass() { return s_Singleton->m_RenderPass; }

	private:
		void OnWindowResized(int width, int height);

	private:
		static VKRendererAPI* s_Singleton;

		Ref<VKGraphicsContext> m_GraphicsContext;
		Ref<VKSwapchain> m_Swapchain;
		Ref<VKRenderPass> m_RenderPass;
		std::vector<Ref<VKGraphicsPipeline>> m_GraphicsPipelines;
		Ref<VKBuffer> m_VertexBuffer;

		std::vector<vk::Semaphore> m_ImageAvailable;
		std::vector<vk::Semaphore> m_RenderFinished;
		std::vector<vk::Fence> m_InFlight;

		bool m_FramebufferInvalid = false;
		bool m_FramebufferResized = false;
	};
}