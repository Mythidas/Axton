#include "axpch.h"
#include "VKImage.h"
#include "VKRendererAPI.h"

namespace Axton::Vulkan
{
	Ref<VKImage> VKImage::Create(vk::Image image, const Specs& specs)
	{
		Ref<VKImage> vkImage = CreateRef<VKImage>();

		// TODO: If image is null create the image
		vkImage->m_Image = image;

		vk::ImageViewCreateInfo createInfo{};
		createInfo
			.setImage(vkImage->m_Image)
			.setViewType(specs.ViewType)
			.setFormat(specs.Format)
			.setComponents({ vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity });
		createInfo.subresourceRange.aspectMask = specs.AspectFlags;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		vkImage->m_ImageView = VKRendererAPI::GetGraphicsContext().GetDevice().createImageView(createInfo);

		return vkImage;
	}

	void VKImage::Destroy()
	{
		if (m_ImageView)
			VKRendererAPI::GetGraphicsContext().GetDevice().destroy(m_ImageView);
	}
}