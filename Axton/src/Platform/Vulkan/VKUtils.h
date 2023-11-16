#pragma once

#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKUtils
	{
	public:
		static uint32_t FindMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags flags);
		static vk::ShaderModule CreateShader(const std::string& path);
		static vk::PipelineShaderStageCreateInfo CreateShaderStageInfo(vk::ShaderModule shader, vk::ShaderStageFlagBits stage);
	};
}