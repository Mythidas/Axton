#include "axpch.h"
#include "VKUtils.h"
#include "VKRenderEngine.h"
#include "Axton/Core/Application.h"
#include "Axton/Utils/FileSystem.h"

namespace Axton::Vulkan
{
	vk::SurfaceFormatKHR VKUtils::ChooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> availableFormats)
	{
		for (const auto& format : availableFormats)
		{
			if (format.format == vk::Format::eB8G8R8A8Snorm && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
			{
				return format;
			}
		}

		return availableFormats[0];
	}

	vk::PresentModeKHR VKUtils::ChoosePresentMode(const std::vector<vk::PresentModeKHR> availableModes)
	{
		for (const auto& mode : availableModes)
		{
			if (mode == vk::PresentModeKHR::eMailbox)
				return mode;
		}

		return vk::PresentModeKHR::eFifo;
	}

	vk::Extent2D VKUtils::ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}
		else
		{
			vk::Extent2D actualExtent =
			{
				Application::Get().GetWindow()->GetWidth(),
				Application::Get().GetWindow()->GetHeight()
			};

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

	uint32_t VKUtils::FindMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags flags)
	{
		vk::PhysicalDevice device = VKRenderEngine::GetPhysicalDevice();

		vk::PhysicalDeviceMemoryProperties memProperties = device.getMemoryProperties();

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & flags) == flags)
				return i;
		}

		AssertCore(false, "Failed to find suitable MemoryType!");
		return 0;
	}

	vk::ShaderModule VKUtils::CreateShader(const std::string& path)
	{
		vk::Device device = VKRenderEngine::GetDevice();

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