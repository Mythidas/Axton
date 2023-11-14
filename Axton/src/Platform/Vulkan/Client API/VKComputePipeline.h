#pragma once

#include "Axton/Renderer/ComputePipeline.h"

#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKComputePipeline : public ComputePipeline
	{
	public:
		VKComputePipeline(const Specs& specs);

		virtual void Dispatch(uint32_t width, uint32_t height, uint32_t depth) override;

	private:
		void createDescriptorPool();
		void createDescriptorSetLayout();
		void createDescriptorSets();
		void createPipelineLayout();
		void createPipeline();

	private:
		Specs m_Specs;

		vk::Pipeline m_Pipeline;
		vk::PipelineLayout m_Layout;

		vk::DescriptorPool m_DescriptorPool;
		std::vector<vk::DescriptorSet> m_DescriptorSets;
		vk::DescriptorSetLayout m_DescriptorSetLayout;
	};
}