#include "axpch.h"
#include "VKUtils.h"
#include "VKRenderEngine.h"
#include "Axton/Utils/FileSystem.h"

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

	vk::ShaderModule VKUtils::CreateShader(const std::string& path)
	{
		vk::Device device = VKRenderEngine::GetGraphicsContext()->GetDevice();

		FileSystem fs(path);
		std::vector<char> buffer = fs.ToSignedBuffer();

		vk::ShaderModuleCreateInfo createInfo{};
		createInfo
			.setCodeSize(buffer.size())
			.setPCode(reinterpret_cast<const uint32_t*>(buffer.data()));

		return device.createShaderModule(createInfo);
	}

	vk::PipelineShaderStageCreateInfo VKUtils::CreateShaderStageInfo(vk::ShaderModule shader, vk::ShaderStageFlagBits stage)
	{
		vk::PipelineShaderStageCreateInfo createInfo{};
		createInfo
			.setStage(stage)
			.setModule(shader)
			.setPName("main");

		return createInfo;
	}
}