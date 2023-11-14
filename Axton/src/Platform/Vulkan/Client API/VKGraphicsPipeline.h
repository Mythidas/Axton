#pragma once

#include "Axton/Core/Defines.h"
#include "Axton/Renderer/GraphicsPipeline.h"

#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKGraphicsPipeline : public GraphicsPipeline
	{
	public:
		VKGraphicsPipeline(const Specs& specs);

		virtual void Render(uint32_t count) override;

		vk::Pipeline& GetPipeline() { return m_Pipeline; }
		vk::PipelineLayout& GetPipelineLayout() { return m_Layout; }

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