#pragma once

#include "VKShader.h"
#include "VKGraphicsContext.h"
#include "VKSwapchain.h"

#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKGraphicsPipeline
	{
	public:
		static Ref<VKGraphicsPipeline> Create(Ref<VKGraphicsContext> graphicsContext, Ref<VKSwapchain> swapchain);

		void BeginRender();
		void EndRender();

		vk::RenderPass& GetRenderPass() { return m_RenderPass; }

	private:
		void createPipelineLayout();
		void createRenderPass();
		void createPipeline();

	private:
		vk::Pipeline m_Pipeline;
		vk::PipelineLayout m_Layout;
		vk::RenderPass m_RenderPass;
		Ref<VKGraphicsContext> m_GraphicsContext;
		Ref<VKSwapchain> m_Swapchain;
	};
}