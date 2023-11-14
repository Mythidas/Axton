#include "axpch.h"
#include "VKComputePipeline.h"
#include "VKRenderBuffer.h"
#include "../VKRenderEngine.h"
#include "../VKUtils.h"

namespace Axton::Vulkan
{
	VKComputePipeline::VKComputePipeline(const Specs& specs)
		: m_Specs(specs)
	{
		createPipelineLayout();
		createPipeline();

		VKRenderEngine::GetGraphicsContext()->QueueDeletion([this]()
		{
			VKRenderEngine::GetGraphicsContext()->GetDevice().destroy(m_Pipeline);
			VKRenderEngine::GetGraphicsContext()->GetDevice().destroy(m_Layout);
		});
	}

	void VKComputePipeline::Dispatch(uint32_t width, uint32_t height, uint32_t depth)
	{
		vk::CommandBuffer buffer = VKRenderEngine::GetGraphicsContext()->GetCommandBuffer();

		buffer.bindPipeline(vk::PipelineBindPoint::eCompute, m_Pipeline);
		buffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute, m_Layout, 0, { m_DescriptorSets[VKRenderEngine::GetGraphicsContext()->GetCurrentFrame()] }, {  });
		buffer.dispatch(width, height, depth);
	}

	void VKComputePipeline::createDescriptorPool()
	{
		std::vector<vk::DescriptorPoolSize> poolSizes;

		for (auto& rBuffer : m_Specs.Buffers)
		{
			VKRenderBuffer* vkrBuffer = static_cast<VKRenderBuffer*>(rBuffer.get());
			poolSizes.push_back(vkrBuffer->GetPoolSize());
		}

		vk::DescriptorPoolCreateInfo createInfo{};
		createInfo
			.setPoolSizeCount(static_cast<uint32_t>(poolSizes.size()))
			.setPPoolSizes(poolSizes.data())
			.setMaxSets(static_cast<uint32_t>(VKRenderEngine::MAX_FRAMES_IN_FLIGHT));

		m_DescriptorPool = VKRenderEngine::GetGraphicsContext()->GetDevice().createDescriptorPool(createInfo);
		AX_ASSERT_CORE(m_DescriptorPool, "Failed to create DescriptorPool!");
	}

	void VKComputePipeline::createDescriptorSetLayout()
	{
		std::vector<vk::DescriptorSetLayoutBinding> layoutBindings;

		for (auto& rBuffer : m_Specs.Buffers)
		{
			VKRenderBuffer* vkrBuffer = static_cast<VKRenderBuffer*>(rBuffer.get());
			layoutBindings.push_back(vkrBuffer->GetLayoutBinding());
		}

		vk::DescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo
			.setBindingCount(static_cast<uint32_t>(layoutBindings.size()))
			.setPBindings(layoutBindings.data());

		m_DescriptorSetLayout = VKRenderEngine::GetGraphicsContext()->GetDevice().createDescriptorSetLayout(layoutInfo);
		AX_ASSERT_CORE(m_DescriptorSetLayout, "Failed to create DescriptorSetLayout!");
	}

	void VKComputePipeline::createDescriptorSets()
	{
		std::vector<vk::DescriptorSetLayout> layouts(static_cast<uint32_t>(VKRenderEngine::MAX_FRAMES_IN_FLIGHT), m_DescriptorSetLayout);

		vk::DescriptorSetAllocateInfo allocInfo{};
		allocInfo
			.setDescriptorPool(m_DescriptorPool)
			.setDescriptorSetCount(static_cast<uint32_t>(layouts.size()))
			.setPSetLayouts(layouts.data());

		m_DescriptorSets = VKRenderEngine::GetGraphicsContext()->GetDevice().allocateDescriptorSets(allocInfo);

		for (auto& set : m_DescriptorSets)
		{
			for (auto& buffer : m_Specs.Buffers)
			{
				VKRenderBuffer* rBuffer = static_cast<VKRenderBuffer*>(buffer.get());
				rBuffer->UpdateDescriptorSet(set);
			}
		}
	}

	void VKComputePipeline::createPipelineLayout()
	{
		vk::PipelineLayoutCreateInfo createInfo{};
		createInfo
			.setSetLayoutCount(1)
			.setPSetLayouts(&m_DescriptorSetLayout);

		m_Layout = VKRenderEngine::GetGraphicsContext()->GetDevice().createPipelineLayout(createInfo);
		AX_ASSERT_CORE(m_Layout, "Failed to create PipelineLayout! (ComputePipeline)");
	}

	void VKComputePipeline::createPipeline()
	{
		vk::ShaderModule compute = VKUtils::CreateShader(m_Specs.ShaderPath);
		auto computeStageInfo = VKUtils::CreateShaderStageInfo(compute, vk::ShaderStageFlagBits::eCompute);

		vk::ComputePipelineCreateInfo createInfo{};
		createInfo
			.setLayout(m_Layout)
			.setStage(computeStageInfo);

		m_Pipeline = VKRenderEngine::GetGraphicsContext()->GetDevice().createComputePipelines(VK_NULL_HANDLE, { createInfo }).value[0];
		AX_ASSERT_CORE(m_Pipeline, "Failed to create Pipeline! (ComputePipeline)");
	}
}