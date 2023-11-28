#pragma once

#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKUtils
	{
	public:
		static vk::SurfaceFormatKHR ChooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> availableFormats);
		static vk::PresentModeKHR ChoosePresentMode(const std::vector<vk::PresentModeKHR> availableModes);
		static vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);

		static uint32_t FindMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags flags);
		static vk::ShaderModule CreateShader(const std::string& path);
		static vk::PipelineShaderStageCreateInfo CreateShaderStageInfo(vk::ShaderModule shader, vk::ShaderStageFlagBits stage);
	};
}