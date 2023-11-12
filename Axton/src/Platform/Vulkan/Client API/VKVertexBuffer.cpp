#include "axpch.h"
#include "VKVertexBuffer.h"
#include "../VKRenderEngine.h"

namespace Axton::Vulkan
{
	VKVertexBuffer::VKVertexBuffer(const Specs& specs)
	{
		m_Buffer = VKBuffer::Specs()
			.setBinding(0)
			.setUsage(vk::BufferUsageFlagBits::eVertexBuffer)
			.setMemProperties(vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible)
			.setSize(specs.Size)
			.Build();
	}

	void VKVertexBuffer::Bind() const
	{
		VKRenderEngine::GetGraphicsContext()->GetBuffer().bindVertexBuffers(0, { m_Buffer->GetBuffer() }, { 0 });
	}

	void VKVertexBuffer::SetData(void* data, size_t size)
	{
		m_Buffer->SetData(data, size);
	}
}