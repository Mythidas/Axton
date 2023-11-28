#include "axpch.h"
#include "VKRenderBuffer.h"
#include "../VKRenderEngine.h"

namespace Axton::Vulkan
{
	namespace Utils
	{
		vk::BufferUsageFlags usageToVulkan(const Axton::RenderBuffer::Specs& specs)
		{
			vk::BufferUsageFlags flags{};

			switch (specs.Usage)
			{
			case Axton::BufferUsage::Uniform: flags |= vk::BufferUsageFlagBits::eUniformBuffer; break;
			case Axton::BufferUsage::ShaderStorage: flags |= vk::BufferUsageFlagBits::eStorageBuffer; break;
			}

			switch (specs.Storage)
			{
			case Axton::BufferStorage::Local: flags |= vk::BufferUsageFlagBits::eTransferDst; break;
			}
		
			return flags;
		}

		vk::MemoryPropertyFlags storageToVulkan(Axton::BufferStorage storage)
		{
			switch (storage)
			{
			case Axton::BufferStorage::Host: return vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
			case Axton::BufferStorage::Local: return vk::MemoryPropertyFlagBits::eDeviceLocal;
			}

			return vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
		}

		vk::ShaderStageFlags stageToVulkan(Axton::BufferStage stage)
		{
			vk::ShaderStageFlags flags{};

			if (stage & Axton::BufferStage::Vertex) flags |= vk::ShaderStageFlagBits::eVertex;
			if (stage & Axton::BufferStage::Fragment) flags |= vk::ShaderStageFlagBits::eFragment;
			if (stage & Axton::BufferStage::Compute) flags |= vk::ShaderStageFlagBits::eCompute;

			if (uint32_t(flags) == 0U) 
				return vk::ShaderStageFlagBits::eAll;

			return flags;
		}

		vk::DescriptorType getDescriptorType(Axton::BufferUsage usage)
		{
			switch (usage)
			{
			case Axton::BufferUsage::Uniform: return vk::DescriptorType::eUniformBuffer;
			case Axton::BufferUsage::ShaderStorage: return vk::DescriptorType::eStorageBuffer;
			}

			return vk::DescriptorType::eUniformBuffer;
		}
	}

	VKRenderBuffer::VKRenderBuffer(const Specs& specs)
		: m_Specs(specs)
	{
		if (specs.Rate == BufferRate::PerFrame)
			m_Buffers.resize(static_cast<size_t>(VKRenderEngine::MAX_FRAMES_IN_FLIGHT));
		else
			m_Buffers.resize(1);

		for (size_t i = 0; i < m_Buffers.size(); i++)
		{
			m_Buffers[i] = VKBuffer::Specs()
				.setSize(specs.Size)
				.setUsage(Utils::usageToVulkan(specs))
				.setMemProperties(Utils::storageToVulkan(specs.Storage))
				.Build();
		}
	}

	void VKRenderBuffer::SetData(void* data, size_t size, uint32_t offset)
	{
		getCurrentBuffer()->SetData(data, size, offset);
	}

	void* VKRenderBuffer::MapBufferPtr(uint32_t offset, size_t size)
	{
		return getCurrentBuffer()->MapBufferPtr(offset, size);
	}

	void VKRenderBuffer::UnmapBufferPtr()
	{
		getCurrentBuffer()->UnmapBufferPtr();
	}

	void VKRenderBuffer::UpdateDescriptorSet(vk::DescriptorSet& set)
	{
		for (auto& buffer : m_Buffers)
		{
			vk::DescriptorBufferInfo bufferInfo{};
			bufferInfo
				.setBuffer(buffer->GetBuffer())
				.setOffset(0)
				.setRange(buffer->GetSpecs().Size);

			vk::WriteDescriptorSet descriptorWrite{};
			descriptorWrite
				.setDstSet(set)
				.setDstBinding(m_Specs.Binding)
				.setDstArrayElement(0)
				.setDescriptorType(Utils::getDescriptorType(m_Specs.Usage))
				.setDescriptorCount(1)
				.setPBufferInfo(&bufferInfo);

			VKRenderEngine::GetDevice().updateDescriptorSets({ descriptorWrite }, {  });
		}
	}

	vk::DescriptorPoolSize VKRenderBuffer::GetPoolSize()
	{
		uint32_t count = m_Specs.Rate == BufferRate::PerFrame ? static_cast<uint32_t>(VKRenderEngine::MAX_FRAMES_IN_FLIGHT) : 1;
		vk::DescriptorPoolSize poolSize{};
		poolSize
			.setDescriptorCount(count)
			.setType(Utils::getDescriptorType(m_Specs.Usage));

		return poolSize;
	}

	vk::DescriptorSetLayoutBinding VKRenderBuffer::GetLayoutBinding()
	{
		vk::DescriptorSetLayoutBinding layoutBinding{};
		layoutBinding
			.setDescriptorCount(1)
			.setBinding(m_Specs.Binding)
			.setDescriptorType(Utils::getDescriptorType(m_Specs.Usage))
			.setStageFlags(Utils::stageToVulkan(m_Specs.Stages))
			.setPImmutableSamplers(nullptr);

		return layoutBinding;
	}

	Ref<VKBuffer> VKRenderBuffer::getCurrentBuffer() const
	{
		if (m_Buffers.size() > 1)
			return m_Buffers[VKRenderEngine::GetCurrentFrame()];
		else
			return m_Buffers[0];
	}
}