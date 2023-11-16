#pragma once

#include "Axton/Core/Defines.h"

#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKBuffer
	{
	public:
		struct Specs
		{
			size_t Size{ 0 };
			vk::BufferUsageFlags Usage{};
			vk::MemoryPropertyFlags MemProperties{};
			bool Staging{ false };

			Specs& setSize(size_t size) { Size = size; return *this; }
			Specs& setUsage(vk::BufferUsageFlags usage) { Usage = usage; return *this; }
			Specs& setMemProperties(vk::MemoryPropertyFlags props) { MemProperties = props; return *this; }
			Specs& setStaging(bool staging) { Staging = staging; return *this; }
			Ref<VKBuffer> Build() { return VKBuffer::Create(*this); }
		};

		static Ref<VKBuffer> Create(const Specs& specs);
		~VKBuffer();

		void SetData(void* data, size_t size, uint32_t offset);
		void* MapBufferPtr(uint32_t offset, size_t size);
		void UnmapBufferPtr();

		VKBuffer::Specs& GetSpecs() { return m_Specs; }
		vk::Buffer& GetBuffer() { return m_Buffer; }
		vk::DeviceMemory& GetMemory() { return m_Memory; }

	private:
		Specs m_Specs;

		vk::Buffer m_Buffer;
		vk::DeviceMemory m_Memory;

		Ref<VKBuffer> m_StagingBuffer{ nullptr };
		uint32_t m_StagingBufferOffset{ 0 };
	};
}