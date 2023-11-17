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
		VKRenderEngine::GetGraphicsContext()->GetDevice().destroy(m_Pipeline);
		VKRenderEngine::GetGraphicsContext()->GetDevice().destroy(m_Layout);
	}

	void VKRayTracePipeline::createPipelineLayout()
	{

	}

	void VKRayTracePipeline::createPipeline()
	{
		vk::RayTracingPipelineCreateInfoKHR createInfo{};
	}
}