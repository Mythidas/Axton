#pragma once

#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKUtils
	{
	public:
		static uint32_t FindMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags flags);
	};
}