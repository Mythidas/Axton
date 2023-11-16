#pragma once

#include "Axton/Renderer/IndexBuffer.h"
#include "../VKBuffer.h"

namespace Axton::Vulkan
{
	class VKIndexBuffer : public IndexBuffer
	{
	public:
		VKIndexBuffer(const IndexBuffer::Specs& specs);

		virtual void Bind() const override;
		virtual void SetData(void* data, size_t size, uint32_t offset) override;

	private:
		Ref<VKBuffer> getCurrentBuffer() const;

	private:
		IndexBuffer::Specs m_Specs;

		std::vector<Ref<VKBuffer>> m_Buffers;
	};
}