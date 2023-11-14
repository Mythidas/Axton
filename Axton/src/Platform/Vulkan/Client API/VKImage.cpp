#include "axpch.h"
#include "VKImage.h"

namespace Axton::Vulkan
{
	namespace Utils
	{
		vk::Format formatToVulkan(Axton::ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::R8: return vk::Format::eR8Srgb;
			case ImageFormat::RGBA8: return vk::Format::eR8G8B8A8Srgb;
			case ImageFormat::RGBA32F: return vk::Format::eR32G32B32A32Sfloat;
			}

			return vk::Format::eUndefined;
		}

		vk::ImageType typeToVulkan(ImageType type)
		{
			switch (type)
			{
			case ImageType::e1D: return vk::ImageType::e1D;
			case ImageType::e2D: return vk::ImageType::e2D;
			case ImageType::e3D: return vk::ImageType::e3D;
			}

			return vk::ImageType::e2D;
		}

		vk::ImageViewType viewTypeToVulkan(ImageType type)
		{
			switch (type)
			{
			case ImageType::e1D: return vk::ImageViewType::e1D;
			case ImageType::e2D: return vk::ImageViewType::e2D;
			case ImageType::e3D: return vk::ImageViewType::e3D;
			}

			return vk::ImageViewType::e2D;
		}

		vk::ImageLayout layoutToVulkan(ImageUsage usage)
		{
			if (usage & ImageUsage::ShaderWrite)
				return vk::ImageLayout::eGeneral;

			return vk::ImageLayout::eShaderReadOnlyOptimal;
		}

		vk::ImageUsageFlags usageToVulkan(ImageUsage usage)
		{
			vk::ImageUsageFlags flags;

			if (usage & ImageUsage::ShaderRead)
				flags |= vk::ImageUsageFlagBits::eSampled;
			if (usage & ImageUsage::HostWrite)
				flags |= vk::ImageUsageFlagBits::eTransferDst;
			if (usage & ImageUsage::ShaderWrite)
				flags |= vk::ImageUsageFlagBits::eStorage;

			return flags;
		}

		vk::AccessFlags accessToVulkan(ImageUsage usage)
		{
			vk::AccessFlags flags;

			if (usage & ImageUsage::ShaderRead)
				flags |= vk::AccessFlagBits::eShaderRead;
			if (usage & ImageUsage::ShaderWrite)
				flags |= vk::AccessFlagBits::eShaderWrite;
			if (usage & ImageUsage::HostRead)
				flags |= vk::AccessFlagBits::eHostRead;
			if (usage & ImageUsage::HostWrite)
				flags |= vk::AccessFlagBits::eHostWrite;

			return flags;
		}

		vk::PipelineStageFlags stagesToVulkan(ImageStages stages)
		{
			vk::PipelineStageFlags flags;

			if (stages & ImageStages::Graphics)
				flags |= vk::PipelineStageFlagBits::eAllGraphics;
			if (stages & ImageStages::Compute)
				flags |= vk::PipelineStageFlagBits::eComputeShader;
			if (stages & ImageStages::RayTracing)
			{
				flags |= vk::PipelineStageFlagBits::eRayTracingShaderKHR;
				flags |= vk::PipelineStageFlagBits::eRayTracingShaderNV;
			}

			return flags;
		}
	}

	VKImage::VKImage(const Specs& specs)
		: m_Specs(specs)
	{
		m_ImageCore = VKImageCore::Specs()
			.setFormat(Utils::formatToVulkan(specs.Format))
			.setImageType(Utils::typeToVulkan(specs.Type))
			.setExtent(vk::Extent3D(specs.Extents.x, specs.Extents.y, specs.Extents.z))
			.setViewType(Utils::viewTypeToVulkan(specs.Type))
			.setLayout(Utils::layoutToVulkan(specs.Usage))
			.setUsage(Utils::usageToVulkan(specs.Usage))
			.setAspectFlags(vk::ImageAspectFlagBits::eColor)
			.setDstMask(Utils::accessToVulkan(specs.Usage))
			.setDstStage(Utils::stagesToVulkan(specs.Stages))
			.Build();
	}

	void VKImage::SetData(void* data, size_t size)
	{

	}
}