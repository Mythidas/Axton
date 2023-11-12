#include "axpch.h"
#include "VKRenderBuffer.h"
#include "../VKRenderEngine.h"

namespace Axton::Vulkan
{
	namespace Utils
	{
		vk::BufferUsageFlagBits usageToVulkan(Axton::BufferUsage usage)
		{
			switch (usage)
			{
			case Axton::Vertex: return vk::BufferUsageFlagBits::eVertexBuffer;
			case Axton::Index: return vk::BufferUsageFlagBits::eIndexBuffer;
			case Axton::Uniform: return vk::BufferUsageFlagBits::eUniformBuffer;
			case Axton::ShaderStorage: return vk::BufferUsageFlagBits::eStorageBuffer;
			}

			AX_ASSERT_CORE(false, "Unknown BufferUsage!");
			return vk::BufferUsageFlagBits::eTransferDst;
		}
	}

	VKRenderBuffer::VKRenderBuffer(const Specs& specs)
		: m_Specs(specs)
	{
		m_Buffer = VKBuffer::Specs()
			.setBinding(0)
			.setSize(specs.Size)
			.setUsage(Utils::usageToVulkan(specs.Usage))
			.setMemProperties(vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible)
			.Build();
	}

	void VKRenderBuffer::Bind() const
	{
		if (m_Specs.Usage == Vertex)
		{
			VKRenderEngine::GetGraphicsContext()->GetCommandBuffer().bindVertexBuffers(0, { m_Buffer->GetBuffer() }, { 0 });
		}
		else if (m_Specs.Usage == Index)
		{
			VKRenderEngine::GetGraphicsContext()->GetCommandBuffer().bindIndexBuffer(m_Buffer->GetBuffer(), 0, vk::IndexType::eUint32);
		}
	}

	void VKRenderBuffer::SetData(void* data, size_t size)
	{
		m_Buffer->SetData(data, size);
	}
}