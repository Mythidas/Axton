#pragma once

#include "Axton/Core/Defines.h"

#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKComputePipeline
	{
	public:
		static Ref<VKComputePipeline> Create();

		void Process();

	private:
		void createPipelineLayout();
		void createPipeline();

	private:
		vk::Pipeline m_Pipeline;
		vk::PipelineLayout m_Layout;
	};
}