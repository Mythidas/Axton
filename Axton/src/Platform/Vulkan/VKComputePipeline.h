#pragma once

#include "VKGraphicsContext.h"
#include "Axton/Core/Defines.h"

#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKComputePipeline
	{
	public:
		static Ref<VKComputePipeline> Create(Ref<VKGraphicsContext> graphicsContext);

	private:
		void createPipelineLayout();
		void createShaderStage();
		void createPipeline();

	private:
		vk::Pipeline m_Pipeline;
		vk::PipelineLayout m_Layout;

		Ref<VKGraphicsContext> m_GraphicsContext;
	};
}