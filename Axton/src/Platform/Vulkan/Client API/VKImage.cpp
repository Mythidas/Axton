#include "axpch.h"
#include "VKImage.h"
#include "../VKRenderEngine.h"
#include "Axton/Math/Mathf.h"

#include <backends/imgui_impl_vulkan.h>

namespace Axton::Vulkan
{
	namespace Utils
	{
		vk::Format formatToVulkan(Axton::ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::R8: return vk::Format::eR8Srgb;
			case ImageFormat::RGBA8: return vk::Format::eR8G8B8A8Snorm;
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
			}

			return flags;
		}

		vk::DescriptorType descripTypeToVulkan(ImageUsage usage)
		{
			if (usage & ImageUsage::ShaderWrite)
				return vk::DescriptorType::eStorageImage;
			else if (usage & ImageUsage::ShaderRead)
				return vk::DescriptorType::eSampler;

			return vk::DescriptorType::eSampledImage;
		}

		vk::ShaderStageFlags shaderStagesToVulkan(ImageStages stage)
		{
			vk::ShaderStageFlags flags{};

			if (stage & ImageStages::Graphics)
				flags |= vk::ShaderStageFlagBits::eAllGraphics;
			if (stage & ImageStages::Compute)
				flags |= vk::ShaderStageFlagBits::eCompute;
			if (stage & ImageStages::RayTracing)
				flags |= vk::ShaderStageFlagBits::eRaygenKHR;

			return flags;
		}
	}

	VKImage::VKImage(const Specs& specs)
		: m_Specs(specs)
	{
		UVector3 extents = specs.Extents;
		extents.x = Mathf::Clamp(extents.x, 1U, extents.x);
		extents.y = Mathf::Clamp(extents.y, 1U, extents.y);
		extents.z = Mathf::Clamp(extents.z, 1U, extents.z);

		m_ImageCore = VKImageCore::Specs()
			.setFormat(Utils::formatToVulkan(specs.Format))
			.setImageType(Utils::typeToVulkan(specs.Type))
			.setExtent(vk::Extent3D(extents.x, extents.y, extents.z))
			.setViewType(Utils::viewTypeToVulkan(specs.Type))
			.setLayout(Utils::layoutToVulkan(specs.Usage))
			.setUsage(Utils::usageToVulkan(specs.Usage))
			.setAspectFlags(vk::ImageAspectFlagBits::eColor)
			.setDstMask(Utils::accessToVulkan(specs.Usage))
			.setDstStage(Utils::stagesToVulkan(specs.Stages))
			.Build();

		m_DescriptorSet = (VkDescriptorSet)ImGui_ImplVulkan_AddTexture(m_ImageCore->GetSampler(), m_ImageCore->GetView(), VK_IMAGE_LAYOUT_GENERAL);
	}

	void VKImage::Resize(UVector3 extents)
	{
		extents.x = Mathf::Clamp(extents.x, 1U, extents.x);
		extents.y = Mathf::Clamp(extents.y, 1U, extents.y);
		extents.z = Mathf::Clamp(extents.z, 1U, extents.z);

		if (extents == m_Specs.Extents) return;

		m_Specs.Extents = extents;

		m_ImageCore->Destroy();
		m_ImageCore = VKImageCore::Specs()
			.setFormat(Utils::formatToVulkan(m_Specs.Format))
			.setImageType(Utils::typeToVulkan(m_Specs.Type))
			.setExtent(vk::Extent3D(m_Specs.Extents.x, m_Specs.Extents.y, m_Specs.Extents.z))
			.setViewType(Utils::viewTypeToVulkan(m_Specs.Type))
			.setLayout(Utils::layoutToVulkan(m_Specs.Usage))
			.setUsage(Utils::usageToVulkan(m_Specs.Usage))
			.setAspectFlags(vk::ImageAspectFlagBits::eColor)
			.setDstMask(Utils::accessToVulkan(m_Specs.Usage))
			.setDstStage(Utils::stagesToVulkan(m_Specs.Stages))
			.Build();

		m_DescriptorSet = (VkDescriptorSet)ImGui_ImplVulkan_AddTexture(m_ImageCore->GetSampler(), m_ImageCore->GetView(), VK_IMAGE_LAYOUT_GENERAL);
	}

	void VKImage::SetData(void* data, size_t size)
	{

	}

	void VKImage::UpdateDescriptorSet(vk::DescriptorSet& set)
	{
		vk::DescriptorImageInfo imageInfo{};
		imageInfo
			.setImageLayout(Utils::layoutToVulkan(m_Specs.Usage))
			.setImageView(m_ImageCore->GetView())
			.setSampler(m_ImageCore->GetSampler());

		vk::WriteDescriptorSet descriptorWrite{};
		descriptorWrite
			.setDstSet(set)
			.setDstBinding(m_Specs.Binding)
			.setDstArrayElement(0)
			.setDescriptorType(Utils::descripTypeToVulkan(m_Specs.Usage))
			.setDescriptorCount(1)
			.setPImageInfo(&imageInfo);

		VKRenderEngine::GetGraphicsContext()->GetDevice().updateDescriptorSets({ descriptorWrite }, {  });
	}

	vk::DescriptorPoolSize VKImage::GetPoolSize()
	{
		vk::DescriptorPoolSize poolSize{};
		poolSize
			.setDescriptorCount(1)
			.setType(Utils::descripTypeToVulkan(m_Specs.Usage));

		return poolSize;
	}

	vk::DescriptorSetLayoutBinding VKImage::GetLayoutBinding()
	{
		vk::DescriptorSetLayoutBinding layoutBinding{};
		layoutBinding
			.setDescriptorCount(1)
			.setBinding(m_Specs.Binding)
			.setDescriptorType(Utils::descripTypeToVulkan(m_Specs.Usage))
			.setStageFlags(Utils::shaderStagesToVulkan(m_Specs.Stages));

		return layoutBinding;
	}

	void VKImage::createImage(UVector3 extents)
	{
		extents.x = Mathf::Clamp(extents.x, 1U, extents.x);
		extents.y = Mathf::Clamp(extents.y, 1U, extents.y);
		extents.z = Mathf::Clamp(extents.z, 1U, extents.z);

		if (extents == m_Specs.Extents) return;

		m_Specs.Extents = extents;

		m_ImageCore->Destroy();
		m_ImageCore = VKImageCore::Specs()
			.setFormat(Utils::formatToVulkan(m_Specs.Format))
			.setImageType(Utils::typeToVulkan(m_Specs.Type))
			.setExtent(vk::Extent3D(m_Specs.Extents.x, m_Specs.Extents.y, m_Specs.Extents.z))
			.setViewType(Utils::viewTypeToVulkan(m_Specs.Type))
			.setLayout(Utils::layoutToVulkan(m_Specs.Usage))
			.setUsage(Utils::usageToVulkan(m_Specs.Usage))
			.setAspectFlags(vk::ImageAspectFlagBits::eColor)
			.setDstMask(Utils::accessToVulkan(m_Specs.Usage))
			.setDstStage(Utils::stagesToVulkan(m_Specs.Stages))
			.Build();

		m_DescriptorSet = (VkDescriptorSet)ImGui_ImplVulkan_AddTexture(m_ImageCore->GetSampler(), m_ImageCore->GetView(), VK_IMAGE_LAYOUT_GENERAL);
	}
}