#pragma once

#include "Axton/Renderer/PipelineAssets.h"

#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKPipelineAssets : public PipelineAssets
	{
	public:
		VKPipelineAssets(const Specs& specs);

		virtual bool Empty() const override;

		vk::DescriptorSet& GetSet();
		vk::DescriptorSetLayout& GetLayout() { return m_DescriptorSetLayout; }

	private:
		void createDescriptorPool();
		void createDescriptorSetLayout();
		void createDescriptorSets();

	private:
		Specs m_Specs;

		vk::DescriptorPool m_DescriptorPool;
		std::vector<vk::DescriptorSet> m_DescriptorSets;
		vk::DescriptorSetLayout m_DescriptorSetLayout;
	};
}