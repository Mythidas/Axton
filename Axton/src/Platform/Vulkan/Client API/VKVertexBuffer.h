#pragma once

#include "Axton/Renderer/VertexBuffer.h"
#include "../VKBuffer.h"

namespace Axton::Vulkan
{
	class VKVertexBuffer : public VertexBuffer
	{
	public:
		VKVertexBuffer(const VertexBuffer::Specs& specs);

		virtual void Bind() const override;
		virtual void SetData(void* data, size_t size, uint32_t offset) override;

	private:
		Ref<VKBuffer> getCurrentBuffer() const;

	private:
		VertexBuffer::Specs m_Specs;

		std::vector<Ref<VKBuffer>> m_Buffers;
	};
}