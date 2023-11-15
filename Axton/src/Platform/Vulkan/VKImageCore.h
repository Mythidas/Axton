#pragma once

#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKImageCore
	{
	public:
		struct Specs
		{
			vk::ImageType ImageType{};
			vk::Extent3D Extent;
			vk::ImageUsageFlags Usage{};
			vk::ImageLayout Layout{};
			vk::Format Format{};

			vk::PipelineStageFlags DstStage;
			vk::AccessFlags DstMask;

			vk::ImageViewType ViewType{};
			vk::ImageAspectFlags AspectFlags{};

			Specs& setImageType(vk::ImageType type) { ImageType = type; return *this; }
			Specs& setExtent(vk::Extent3D extent) { Extent = extent; return *this; }
			Specs& setUsage(vk::ImageUsageFlags usage) { Usage = usage; return *this; }
			Specs& setLayout(vk::ImageLayout layout) { Layout = layout; return *this; }
			Specs& setFormat(vk::Format format) { Format = format; return *this; }

			Specs& setDstStage(vk::PipelineStageFlags stage) { DstStage = stage; return *this; }
			Specs& setDstMask(vk::AccessFlags access) { DstMask = access; return *this; }

			Specs& setViewType(vk::ImageViewType viewType) { ViewType = viewType; return *this; }
			Specs& setAspectFlags(vk::ImageAspectFlags flags) { AspectFlags = flags; return *this; }

			Ref<VKImageCore> Build() { return VKImageCore::Create(*this); }
			Ref<VKImageCore> Build(vk::Image image) { return VKImageCore::Create(image, *this); }
		};

		static Ref<VKImageCore> Create(const Specs& specs);
		static Ref<VKImageCore> Create(vk::Image image, const Specs& specs);

		void Destroy();

		vk::Image& GetImage() { return m_Image; }
		vk::ImageView& GetView() { return m_ImageView; }
		vk::Sampler& GetSampler() { return m_Sampler; }

	private:
		vk::Image m_Image;
		vk::DeviceMemory m_ImageMemory;
		vk::ImageView m_ImageView;
		vk::Sampler m_Sampler;
	};
}