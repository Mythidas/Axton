#pragma once

#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKImage
	{
	public:
		struct Specs
		{
			vk::ImageViewType ViewType;
			vk::Format Format;
			vk::ImageAspectFlags AspectFlags;

			Specs& setViewType(vk::ImageViewType viewType) { ViewType = viewType; return *this; }
			Specs& setFormat(vk::Format format) { Format = format; return *this; }
			Specs& setAspectFlags(vk::ImageAspectFlags flags) { AspectFlags = flags; return *this; }

			Ref<VKImage> Build() { return VKImage::Create(VK_NULL_HANDLE, *this); }
			Ref<VKImage> Build(vk::Image image) { return VKImage::Create(image, *this); }
		};

		static Ref<VKImage> Create(vk::Image image, const Specs& specs);

		void Destroy();

		vk::Image& GetImage() { return m_Image; }
		vk::ImageView& GetView() { return m_ImageView; }

	private:
		vk::Image m_Image;
		vk::ImageView m_ImageView;
	};
}