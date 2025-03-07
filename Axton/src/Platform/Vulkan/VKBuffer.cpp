#include "axpch.h"
#include "VKBuffer.h"
#include "VKRenderEngine.h"
#include "VKUtils.h"

namespace Axton::Vulkan
{
	Ref<VKBuffer> VKBuffer::Create(const Specs& specs)
	{
		Ref<VKBuffer> buffer = CreateRef<VKBuffer>();
		buffer->m_Specs = specs;
		vk::Device device = VKRenderEngine::GetDevice();

		vk::BufferCreateInfo bufferInfo{};
		bufferInfo
			.setSize(specs.Size)
			.setUsage(specs.Usage)
			.setSharingMode(vk::SharingMode::eExclusive);

		buffer->m_Buffer = device.createBuffer(bufferInfo);

		vk::MemoryRequirements memRequirements = device.getBufferMemoryRequirements(buffer->m_Buffer);

		vk::MemoryAllocateInfo allocInfo{};
		allocInfo
			.setAllocationSize(memRequirements.size)
			.setMemoryTypeIndex(VKUtils::FindMemoryType(memRequirements.memoryTypeBits, specs.MemProperties));

		buffer->m_Memory = device.allocateMemory(allocInfo);
		device.bindBufferMemory(buffer->m_Buffer, buffer->m_Memory, 0);

		return buffer;
	}

	VKBuffer::~VKBuffer()
	{
		VKRenderEngine::GetDevice().destroy(m_Buffer);
		VKRenderEngine::GetDevice().freeMemory(m_Memory);
	}

	void VKBuffer::SetData(void* data, size_t size, uint32_t offset)
	{
		AssertCore(size + offset <= m_Specs.Size, "Size is larger than allocated buffer!");
		vk::Device device = VKRenderEngine::GetDevice();

		if (m_Specs.MemProperties & vk::MemoryPropertyFlagBits::eDeviceLocal)
		{
			Ref<VKBuffer> stagingBuffer = Specs()
				.setSize(size)
				.setUsage(vk::BufferUsageFlagBits::eTransferSrc)
				.setMemProperties(vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)
				.setStaging(true)
				.Build();

			void* pData = device.mapMemory(stagingBuffer->m_Memory, offset, size);
			memcpy(pData, data, size);
			device.unmapMemory(stagingBuffer->m_Memory);

			VKRenderEngine::GetGraphicsContext()->SubmitGraphicsCommand([stagingBuffer, offset, this](vk::CommandBuffer& commandBuffer)
			{
				vk::BufferCopy copyRegion{};
				copyRegion
					.setSrcOffset(0)
					.setDstOffset(offset)
					.setSize(stagingBuffer->m_Specs.Size);

				commandBuffer.copyBuffer(stagingBuffer->m_Buffer, m_Buffer, { copyRegion });
			});
		}
		else if (m_Specs.MemProperties & vk::MemoryPropertyFlagBits::eHostVisible && m_Specs.MemProperties & vk::MemoryPropertyFlagBits::eHostCoherent)
		{
			void* pData = device.mapMemory(m_Memory, offset, size);
			memcpy(pData, data, size);
			device.unmapMemory(m_Memory);
		}
		else
		{
			AssertCore(false, "Buffer MemProperties not supported for SetData!");
		}
	}

	void* VKBuffer::MapBufferPtr(uint32_t offset, size_t size)
	{
		AssertCore(size + offset <= m_Specs.Size, "Size is larger than allocated buffer!");
		vk::Device device = VKRenderEngine::GetDevice();

		if (m_Specs.MemProperties & vk::MemoryPropertyFlagBits::eDeviceLocal)
		{
			m_StagingBuffer = Specs()
				.setSize(size)
				.setUsage(vk::BufferUsageFlagBits::eTransferSrc)
				.setMemProperties(vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)
				.setStaging(true)
				.Build();

			m_StagingBufferOffset = offset;
			return device.mapMemory(m_StagingBuffer->m_Memory, 0, size);
		}
		else if (m_Specs.MemProperties & vk::MemoryPropertyFlagBits::eHostVisible && m_Specs.MemProperties & vk::MemoryPropertyFlagBits::eHostCoherent)
		{
			return device.mapMemory(m_Memory, offset, size);
		}
		else
		{
			AssertCore(false, "Buffer MemProperties not supported for SetData!");
			return nullptr;
		}
	}

	void VKBuffer::UnmapBufferPtr()
	{
		vk::Device device = VKRenderEngine::GetDevice();

		if (m_Specs.MemProperties & vk::MemoryPropertyFlagBits::eDeviceLocal)
		{
			device.unmapMemory(m_StagingBuffer->m_Memory);

			// TODO: This might need the dst offset for unmapping?
			VKRenderEngine::GetGraphicsContext()->SubmitGraphicsCommand([this](vk::CommandBuffer& commandBuffer)
			{
				vk::BufferCopy copyRegion{};
				copyRegion
					.setSrcOffset(0)
					.setDstOffset(m_StagingBufferOffset)
					.setSize(m_StagingBuffer->m_Specs.Size);

				commandBuffer.copyBuffer(m_StagingBuffer->m_Buffer, m_Buffer, { copyRegion });
			});
		}
		else if (m_Specs.MemProperties & vk::MemoryPropertyFlagBits::eHostVisible && m_Specs.MemProperties & vk::MemoryPropertyFlagBits::eHostCoherent)
		{
			device.unmapMemory(m_Memory);
		}
		else
		{
			AssertCore(false, "Buffer MemProperties not supported for SetData!");
		}
	}
}