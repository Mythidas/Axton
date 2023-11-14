#include "axpch.h"
#include "VKImageCore.h"
#include "VKRenderEngine.h"
#include "VKUtils.h"

namespace Axton::Vulkan
{
	namespace Utils
	{
		void transitionLayout(vk::Image image, vk::ImageLayout current, vk::AccessFlags srcMask, vk::PipelineStageFlags srcStage, vk::ImageLayout target, vk::AccessFlags dstMask, vk::PipelineStageFlags dstStage)
		{
			vk::ImageMemoryBarrier barrier{};
			barrier
				.setOldLayout(current)
				.setNewLayout(target)
				.setImage(image)
				.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
				.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
				.setSrcAccessMask(srcMask)
				.setDstAccessMask(dstMask);
			barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = 1;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = 1;

			VKRenderEngine::GetGraphicsContext()->SubmitCommand([barrier, srcStage, dstStage](vk::CommandBuffer& buffer)
			{
					buffer.pipelineBarrier(
						srcStage, dstStage,
						vk::DependencyFlags::Flags(),
						{},
						{},
						{ barrier }
					);
			});
		}
	}

	Ref<VKImageCore> VKImageCore::Create(const Specs& specs)
	{
		vk::Device device = VKRenderEngine::GetGraphicsContext()->GetDevice();
		Ref<VKImageCore> vkImage = CreateRef<VKImageCore>();

		vk::ImageCreateInfo imageInfo{};
		imageInfo
			.setFormat(specs.Format)
			.setImageType(specs.ImageType)
			.setUsage(specs.Usage)
			.setExtent(specs.Extent)
			.setInitialLayout(vk::ImageLayout::eUndefined)
			.setMipLevels(1)
			.setArrayLayers(1)
			.setTiling(vk::ImageTiling::eOptimal)
			.setSharingMode(vk::SharingMode::eExclusive)
			.setSamples(vk::SampleCountFlagBits::e1);

		vkImage->m_Image = device.createImage(imageInfo);

		vk::MemoryRequirements memRequirements = device.getImageMemoryRequirements(vkImage->m_Image);

		vk::MemoryAllocateInfo allocInfo{};
		allocInfo
			.setAllocationSize(memRequirements.size)
			.setMemoryTypeIndex(VKUtils::FindMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal));


		vkImage->m_ImageMemory = device.allocateMemory(allocInfo);
		device.bindImageMemory(vkImage->m_Image, vkImage->m_ImageMemory, 0);
		Utils::transitionLayout(vkImage->m_Image, vk::ImageLayout::eUndefined, vk::AccessFlagBits::eNone, vk::PipelineStageFlagBits::eAllCommands, specs.Layout, specs.DstMask, specs.DstStage);

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

		vkImage->m_ImageView = device.createImageView(createInfo);

		VKRenderEngine::GetGraphicsContext()->QueueDeletion([device, vkImage]() {
			device.destroy(vkImage->m_Image);
			device.destroy(vkImage->m_ImageView);
			device.freeMemory(vkImage->m_ImageMemory);
		});

		return vkImage;
	}

	Ref<VKImageCore> VKImageCore::Create(vk::Image image, const Specs& specs)
	{
		Ref<VKImageCore> vkImage = CreateRef<VKImageCore>();

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

		vkImage->m_ImageView = VKRenderEngine::GetGraphicsContext()->GetDevice().createImageView(createInfo);

		VKRenderEngine::GetGraphicsContext()->QueueDeletion([vkImage]() {
			VKRenderEngine::GetGraphicsContext()->GetDevice().destroy(vkImage->m_Image);
			VKRenderEngine::GetGraphicsContext()->GetDevice().destroy(vkImage->m_ImageView);
		});

		return vkImage;
	}

	void VKImageCore::Destroy()
	{
		if (m_ImageView)
			VKRenderEngine::GetGraphicsContext()->GetDevice().destroy(m_ImageView);
	}
}