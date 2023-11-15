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

			VKRenderEngine::GetGraphicsContext()->SubmitGraphicsCommand([barrier, srcStage, dstStage](vk::CommandBuffer& buffer)
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
		AX_ASSERT_CORE(vkImage->m_Image, "Failed to create Image!");

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
		AX_ASSERT_CORE(vkImage->m_ImageView, "Failed to create ImageView!");

		vk::SamplerCreateInfo samplerInfo{};
		samplerInfo
			.setMagFilter(vk::Filter::eLinear)
			.setMinFilter(vk::Filter::eLinear)
			.setMipmapMode(vk::SamplerMipmapMode::eLinear)
			.setAddressModeU(vk::SamplerAddressMode::eRepeat)
			.setAddressModeV(vk::SamplerAddressMode::eRepeat)
			.setAddressModeW(vk::SamplerAddressMode::eRepeat)
			.setMinLod(-1000)
			.setMaxLod(1000)
			.setMaxAnisotropy(1.0f);

		vkImage->m_Sampler = device.createSampler(samplerInfo);
		AX_ASSERT_CORE(vkImage->m_Sampler, "Failed to create Sampler!");

		VKRenderEngine::GetGraphicsContext()->QueueDeletion([device, vkImage]() {
			device.destroy(vkImage->m_Image);
			device.destroy(vkImage->m_ImageView);
			device.destroy(vkImage->m_Sampler);
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