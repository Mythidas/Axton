#include "axpch.h"
#include "VKPipelineAssets.h"
#include "VKRenderBuffer.h"
#include "VKTexture.h"
#include "../VKRenderEngine.h"

namespace Axton::Vulkan
{
	VKPipelineAssets::VKPipelineAssets(const Specs& specs)
		: m_Specs(specs)
	{
		if (Empty()) return;

		createDescriptorPool();
		createDescriptorSetLayout();
		createDescriptorSets();
	}

	VKPipelineAssets::~VKPipelineAssets()
	{
		VKRenderEngine::GetDevice().waitIdle();
		VKRenderEngine::GetDevice().destroy(m_DescriptorPool);
		VKRenderEngine::GetDevice().destroy(m_DescriptorSetLayout);
	}

	void VKPipelineAssets::Rebuild()
	{
		VKRenderEngine::GetDevice().freeDescriptorSets(m_DescriptorPool, m_DescriptorSets);
		createDescriptorSets();
	}

	bool VKPipelineAssets::Empty() const
	{
		return m_Specs.Buffers.empty() && m_Specs.Images.empty();
	}

	vk::DescriptorSet& VKPipelineAssets::GetSet()
	{
		return m_DescriptorSets[VKRenderEngine::GetCurrentFrame()];
	}

	void VKPipelineAssets::createDescriptorPool()
	{
		std::vector<vk::DescriptorPoolSize> poolSizes;

		for (auto& rBuffer : m_Specs.Buffers)
		{
			VKRenderBuffer* vkrBuffer = static_cast<VKRenderBuffer*>(rBuffer.get());
			poolSizes.push_back(vkrBuffer->GetPoolSize());
		}

		for (auto& image : m_Specs.Images)
		{
			VKTexture* vkImage = static_cast<VKTexture*>(image.get());
			poolSizes.push_back(vkImage->GetPoolSize());
		}

		vk::DescriptorPoolCreateInfo createInfo{};
		createInfo
			.setPoolSizeCount(static_cast<uint32_t>(poolSizes.size()))
			.setPPoolSizes(poolSizes.data())
			.setFlags(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet)
			.setMaxSets(static_cast<uint32_t>(VKRenderEngine::MAX_FRAMES_IN_FLIGHT));

		m_DescriptorPool = VKRenderEngine::GetDevice().createDescriptorPool(createInfo);
		AssertCore(m_DescriptorPool, "Failed to create DescriptorPool!");
	}

	void VKPipelineAssets::createDescriptorSetLayout()
	{
		std::vector<vk::DescriptorSetLayoutBinding> layoutBindings;

		for (auto& rBuffer : m_Specs.Buffers)
		{
			VKRenderBuffer* vkrBuffer = static_cast<VKRenderBuffer*>(rBuffer.get());
			layoutBindings.push_back(vkrBuffer->GetLayoutBinding());
		}

		for (auto& image : m_Specs.Images)
		{
			VKTexture* vkImage = static_cast<VKTexture*>(image.get());
			layoutBindings.push_back(vkImage->GetLayoutBinding());
		}

		vk::DescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo
			.setBindingCount(static_cast<uint32_t>(layoutBindings.size()))
			.setPBindings(layoutBindings.data());

		m_DescriptorSetLayout = VKRenderEngine::GetDevice().createDescriptorSetLayout(layoutInfo);
		AssertCore(m_DescriptorSetLayout, "Failed to create DescriptorSetLayout!");
	}

	void VKPipelineAssets::createDescriptorSets()
	{
		std::vector<vk::DescriptorSetLayout> layouts(static_cast<uint32_t>(VKRenderEngine::MAX_FRAMES_IN_FLIGHT), m_DescriptorSetLayout);

		vk::DescriptorSetAllocateInfo allocInfo{};
		allocInfo
			.setDescriptorPool(m_DescriptorPool)
			.setDescriptorSetCount(static_cast<uint32_t>(layouts.size()))
			.setPSetLayouts(layouts.data());

		m_DescriptorSets = VKRenderEngine::GetDevice().allocateDescriptorSets(allocInfo);

		for (auto& set : m_DescriptorSets)
		{
			for (auto& buffer : m_Specs.Buffers)
			{
				VKRenderBuffer* rBuffer = static_cast<VKRenderBuffer*>(buffer.get());
				rBuffer->UpdateDescriptorSet(set);
			}

			for (auto& image : m_Specs.Images)
			{
				VKTexture* vkImage = static_cast<VKTexture*>(image.get());
				vkImage->UpdateDescriptorSet(set);
			}
		}
	}
}