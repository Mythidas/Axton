#include "axpch.h"
#include "VKComputePipeline.h"
#include "VKRenderBuffer.h"
#include "VKPipelineAssets.h"
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

		VKRenderEngine::GetGraphicsContext()->QueueComputeCommand([width, height, depth, this](vk::CommandBuffer& buffer)
		{
			buffer.bindPipeline(vk::PipelineBindPoint::eCompute, m_Pipeline);
			if (!m_Specs.Assets->Empty())
			{
				VKPipelineAssets* assets = static_cast<VKPipelineAssets*>(m_Specs.Assets.get());
				buffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute, m_Layout, 0, { assets->GetSet() }, {});
			}
			buffer.dispatch(width, height, depth);
		});
	}

	void VKComputePipeline::createPipelineLayout()
	{
		vk::DescriptorSetLayout layout = static_cast<VKPipelineAssets*>(m_Specs.Assets.get())->GetLayout();

		vk::PipelineLayoutCreateInfo createInfo{};
		createInfo
			.setSetLayoutCount(layout ? 1 : 0)
			.setPSetLayouts(&layout);

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