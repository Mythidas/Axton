#pragma once

#include "Axton/Renderer/RenderBuffer.h"
#include "../VKBuffer.h"

namespace Axton::Vulkan
{
	class VKRenderBuffer : public RenderBuffer
	{
	public:
		VKRenderBuffer(const Specs& specs);

		virtual void SetData(void* data, size_t size, uint32_t offset) override;
		virtual void* MapBufferPtr(uint32_t offset, size_t size) override;
		virtual void UnmapBufferPtr() override;

		virtual BufferUsage GetUsage() const override { return m_Specs.Usage; }

		void UpdateDescriptorSet(vk::DescriptorSet& set);

		vk::DescriptorPoolSize GetPoolSize();
		vk::DescriptorSetLayoutBinding GetLayoutBinding();

	private:
		Ref<VKBuffer> getCurrentBuffer() const;

	private:
		Specs m_Specs;

		std::vector<Ref<VKBuffer>> m_Buffers;
	};
}