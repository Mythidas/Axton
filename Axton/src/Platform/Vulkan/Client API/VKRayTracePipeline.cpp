#include "axpch.h"
#include "VKRayTracePipeline.h"
#include "../VKRenderEngine.h"

namespace Axton::Vulkan
{
	VKRayTracePipeline::VKRayTracePipeline(const Specs& specs)
	{

	}

	VKRayTracePipeline::~VKRayTracePipeline()
	{
		VKRenderEngine::GetDevice().destroy(m_Pipeline);
		VKRenderEngine::GetDevice().destroy(m_Layout);
	}

	void VKRayTracePipeline::createPipelineLayout()
	{

	}

	void VKRayTracePipeline::createPipeline()
	{
		vk::RayTracingPipelineCreateInfoKHR createInfo{};
	}
}