#include "axpch.h"
#include "VKBuffer.h"
#include "VKRendererAPI.h"
#include "VKUtils.h"

namespace Axton::Vulkan
{
	Ref<VKBuffer> VKBuffer::Create(const Specs& specs)
	{
		Ref<VKBuffer> buffer = CreateRef<VKBuffer>();
		buffer->m_Specs = specs;
		vk::Device device = VKRendererAPI::GetGraphicsContext()->GetDevice();

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

		VKRendererAPI::GetGraphicsContext()->QueueDeletion([device, buffer]()
			{
				device.destroy(buffer->m_Buffer);
				device.freeMemory(buffer->m_Memory);
			});

		return buffer;
	}

	void VKBuffer::Process()
	{
		VKRendererAPI::GetGraphicsContext()->QueueCommand([this](vk::CommandBuffer& buffer)
		{
			buffer.bindVertexBuffers(0, { m_Buffer }, { 0 });
		});
	}

	void VKBuffer::SetData(void* data, size_t size)
	{
		AX_ASSERT_CORE(size <= m_Specs.Size, "Size is larger than allocated buffer!");

		vk::Device device = VKRendererAPI::GetGraphicsContext()->GetDevice();

		void* pData = device.mapMemory(m_Memory, 0, size, vk::MemoryMapFlags::Flags());
		memcpy(pData, data, size);
		device.unmapMemory(m_Memory);
	}
}