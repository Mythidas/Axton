#include "axpch.h"
#include "VKTexture.h"
#include "../VKRenderEngine.h"
#include "../VKImGUILayer.h"
#include "Axton/Math/Mathf.h"
#include "Axton/Core/Application.h"

#include <backends/imgui_impl_vulkan.h>

namespace Axton::Vulkan
{
	namespace Utils
	{
		vk::Format formatToVulkan(Axton::TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::R8: return vk::Format::eR8Srgb;
			case TextureFormat::RGBA8: return vk::Format::eR8G8B8A8Snorm;
			case TextureFormat::RG32F: return vk::Format::eR32G32Sfloat;
			case TextureFormat::RGBA32F: return vk::Format::eR32G32B32A32Sfloat;
			}

			return vk::Format::eUndefined;
		}

		vk::ImageType typeToVulkan(TextureType type)
		{
			switch (type)
			{
			case TextureType::e1D: return vk::ImageType::e1D;
			case TextureType::e2D: return vk::ImageType::e2D;
			case TextureType::e3D: return vk::ImageType::e3D;
			}

			return vk::ImageType::e2D;
		}

		vk::ImageViewType viewTypeToVulkan(TextureType type)
		{
			switch (type)
			{
			case TextureType::e1D: return vk::ImageViewType::e1D;
			case TextureType::e2D: return vk::ImageViewType::e2D;
			case TextureType::e3D: return vk::ImageViewType::e3D;
			}

			return vk::ImageViewType::e2D;
		}

		vk::ImageLayout layoutToVulkan(TextureUsage usage)
		{
			if (usage & TextureUsage::ShaderWrite)
				return vk::ImageLayout::eGeneral;

			return vk::ImageLayout::eShaderReadOnlyOptimal;
		}

		vk::ImageUsageFlags usageToVulkan(TextureUsage usage)
		{
			vk::ImageUsageFlags flags;

			if (usage & TextureUsage::ShaderRead)
				flags |= vk::ImageUsageFlagBits::eSampled;
			if (usage & TextureUsage::HostWrite)
				flags |= vk::ImageUsageFlagBits::eTransferDst;
			if (usage & TextureUsage::ShaderWrite)
				flags |= vk::ImageUsageFlagBits::eStorage;

			return flags;
		}

		vk::AccessFlags accessToVulkan(TextureUsage usage)
		{
			vk::AccessFlags flags;

			if (usage & TextureUsage::ShaderRead)
				flags |= vk::AccessFlagBits::eShaderRead;
			if (usage & TextureUsage::ShaderWrite)
				flags |= vk::AccessFlagBits::eShaderWrite;
			if (usage & TextureUsage::HostRead)
				flags |= vk::AccessFlagBits::eHostRead;
			if (usage & TextureUsage::HostWrite)
				flags |= vk::AccessFlagBits::eHostWrite;

			return flags;
		}

		vk::PipelineStageFlags stagesToVulkan(TextureStages stages)
		{
			vk::PipelineStageFlags flags;

			if (stages & TextureStages::Graphics)
				flags |= vk::PipelineStageFlagBits::eAllGraphics;
			if (stages & TextureStages::Compute)
				flags |= vk::PipelineStageFlagBits::eComputeShader;
			if (stages & TextureStages::RayTracing)
			{
				flags |= vk::PipelineStageFlagBits::eRayTracingShaderKHR;
			}

			return flags;
		}

		vk::DescriptorType descripTypeToVulkan(TextureUsage usage)
		{
			if (usage & TextureUsage::ShaderWrite)
				return vk::DescriptorType::eStorageImage;
			else if (usage & TextureUsage::ShaderRead)
				return vk::DescriptorType::eSampler;

			return vk::DescriptorType::eSampledImage;
		}

		vk::ShaderStageFlags shaderStagesToVulkan(TextureStages stage)
		{
			vk::ShaderStageFlags flags{};

			if (stage & TextureStages::Graphics)
				flags |= vk::ShaderStageFlagBits::eAllGraphics;
			if (stage & TextureStages::Compute)
				flags |= vk::ShaderStageFlagBits::eCompute;
			if (stage & TextureStages::RayTracing)
				flags |= vk::ShaderStageFlagBits::eRaygenKHR;

			return flags;
		}
	}

	VKTexture::VKTexture(const Specs& specs)
		: m_Specs(specs)
	{
		UVector3 extents = specs.Extents;
		extents.x = Mathf::Clamp(extents.x, 1U, extents.x);
		extents.y = Mathf::Clamp(extents.y, 1U, extents.y);
		extents.z = Mathf::Clamp(extents.z, 1U, extents.z);

		m_ImageCore = VKImage::Specs()
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

	bool VKTexture::Resize(UVector3 extents)
	{
		extents.x = Mathf::Clamp(extents.x, 1U, extents.x);
		extents.y = Mathf::Clamp(extents.y, 1U, extents.y);
		extents.z = Mathf::Clamp(extents.z, 1U, extents.z);

		if (extents == m_Specs.Extents)
			return false;

		m_Specs.Extents = extents;

		m_ImageCore = VKImage::Specs()
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

		vk::DescriptorPool imguiPool = static_cast<VKImGUILayer*>(&Application::Get().GetImGUILayer())->GetDescriptorPool();
		VKRenderEngine::GetDevice().freeDescriptorSets(imguiPool, { m_DescriptorSet });
		m_DescriptorSet = (VkDescriptorSet)ImGui_ImplVulkan_AddTexture(m_ImageCore->GetSampler(), m_ImageCore->GetView(), VK_IMAGE_LAYOUT_GENERAL);
		return true;
	}

	void VKTexture::SetData(void* data, size_t size)
	{

	}

	void VKTexture::UpdateDescriptorSet(vk::DescriptorSet& set)
	{
		vk::DescriptorImageInfo TextureInfo{};
		TextureInfo
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
			.setPImageInfo(&TextureInfo);

		VKRenderEngine::GetDevice().updateDescriptorSets({ descriptorWrite }, {  });
	}

	vk::DescriptorPoolSize VKTexture::GetPoolSize()
	{
		vk::DescriptorPoolSize poolSize{};
		poolSize
			.setDescriptorCount(1)
			.setType(Utils::descripTypeToVulkan(m_Specs.Usage));

		return poolSize;
	}

	vk::DescriptorSetLayoutBinding VKTexture::GetLayoutBinding()
	{
		vk::DescriptorSetLayoutBinding layoutBinding{};
		layoutBinding
			.setDescriptorCount(1)
			.setBinding(m_Specs.Binding)
			.setDescriptorType(Utils::descripTypeToVulkan(m_Specs.Usage))
			.setStageFlags(Utils::shaderStagesToVulkan(m_Specs.Stages));

		return layoutBinding;
	}
}