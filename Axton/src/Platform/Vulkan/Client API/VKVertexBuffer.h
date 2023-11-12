#pragma once

#include "Axton/Renderer/VertexBuffer.h"
#include "../VKBuffer.h"

namespace Axton::Vulkan
{
	class VKVertexBuffer : public VertexBuffer
	{
	public:
		VKVertexBuffer(const Specs& specs);

		virtual void Bind() const override;
		virtual void SetData(void* data, size_t size) override;

	private:
		Ref<VKBuffer> m_Buffer;
	};
}