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
			uint32_t Binding{ 0 };
			vk::BufferUsageFlagBits Usage{};
			vk::MemoryPropertyFlags MemProperties{};

			Specs& setSize(size_t size) { Size = size; return *this; }
			Specs& setBinding(uint32_t binding) { Binding = binding; return *this; }
			Specs& setUsage(vk::BufferUsageFlagBits usage) { Usage = usage; return *this; }
			Specs& setMemProperties(vk::MemoryPropertyFlags props) { MemProperties = props; return *this; }
			Ref<VKBuffer> Build() { return VKBuffer::Create(*this); }
		};

		static Ref<VKBuffer> Create(const Specs& specs);

		void SetData(void* data, size_t size);

		vk::Buffer& GetBuffer() { return m_Buffer; }
		vk::DeviceMemory& GetMemory() { return m_Memory; }

	private:
		Specs m_Specs;

		vk::Buffer m_Buffer;
		vk::DeviceMemory m_Memory;
	};
}