#include "axpch.h"
#include "VKUtils.h"
#include "VKRenderEngine.h"

namespace Axton::Vulkan
{
	uint32_t VKUtils::FindMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags flags)
	{
		vk::PhysicalDevice device = VKRenderEngine::GetGraphicsContext()->GetPhysicalDevice();

		vk::PhysicalDeviceMemoryProperties memProperties = device.getMemoryProperties();

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & flags) == flags)
				return i;
		}

		AX_ASSERT_CORE("Failed to find suitable MemoryType!");
		return 0;
	}
}