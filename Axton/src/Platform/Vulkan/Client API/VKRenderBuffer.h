#pragma once

#include "Axton/Renderer/RenderBuffer.h"
#include "../VKBuffer.h"

namespace Axton::Vulkan
{
	class VKRenderBuffer : public RenderBuffer
	{
	public:
		VKRenderBuffer(const Specs& specs);

		virtual void Bind() const override;
		virtual void SetData(void* data, size_t size) override;

		virtual BufferUsage GetUsage() const override { return m_Specs.Usage; }

	private:
		Specs m_Specs;

		Ref<VKBuffer> m_Buffer;
	};
}