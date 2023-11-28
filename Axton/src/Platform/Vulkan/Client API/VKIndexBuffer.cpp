#include "axpch.h"
#include "VKIndexBuffer.h"
#include "../VKRenderEngine.h"

namespace Axton::Vulkan
{
	VKIndexBuffer::VKIndexBuffer(const IndexBuffer::Specs& specs)
	{
		if (specs.Rate == BufferRate::PerFrame)
			m_Buffers.resize(static_cast<size_t>(VKRenderEngine::MAX_FRAMES_IN_FLIGHT));
		else
			m_Buffers.resize(1);

		for (size_t i = 0; i < m_Buffers.size(); i++)
		{
			m_Buffers[i] = VKBuffer::Specs()
				.setSize(specs.Size)
				.setUsage(vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst)
				.setMemProperties(vk::MemoryPropertyFlagBits::eDeviceLocal)
				.Build();
		}
	}

	void VKIndexBuffer::Bind() const
	{
		VKRenderEngine::GetCommandBuffer().bindIndexBuffer(getCurrentBuffer()->GetBuffer(), 0, vk::IndexType::eUint16);
	}

	void VKIndexBuffer::SetData(void* data, size_t size, uint32_t offset)
	{
		getCurrentBuffer()->SetData(data, size, offset);
	}

	Ref<VKBuffer> VKIndexBuffer::getCurrentBuffer() const
	{
		if (m_Specs.Rate == BufferRate::PerFrame)
			return m_Buffers[VKRenderEngine::GetCurrentFrame()];
		else
			return m_Buffers[0];
	}
}