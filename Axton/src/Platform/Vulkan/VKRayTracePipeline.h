#pragma once

#include "Axton/Renderer/RayTracePipeline.h"
#include "VKObject.h"

#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKRayTracePipeline : public RayTracePipeline, public VKObject
	{
	public:
		VKRayTracePipeline(const Specs& specs);
		~VKRayTracePipeline();

		virtual void Destroy();

	private:
		void createPipelineLayout();
		void createPipeline();

	private:
		vk::Pipeline m_Pipeline;
		vk::PipelineLayout m_Layout;
	};
}