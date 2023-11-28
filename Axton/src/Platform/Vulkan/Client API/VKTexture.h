#pragma once

#include "Axton/Renderer/Texture.h"
#include "../VKImage.h"

namespace Axton::Vulkan
{
	class VKTexture : public Texture
	{
	public:
		VKTexture(const Specs& specs);

		virtual bool Resize(UVector3 extents) override;
		virtual void SetData(void* data, size_t size) override;

		virtual UVector3 GetExtents() const override { return m_Specs.Extents; };
		virtual void* GetRendererID() const override { return m_DescriptorSet; };

		void UpdateDescriptorSet(vk::DescriptorSet& set);

		vk::DescriptorPoolSize GetPoolSize();
		vk::DescriptorSetLayoutBinding GetLayoutBinding();

	private:
		Specs m_Specs;

		Ref<VKImage> m_ImageCore;
		vk::DescriptorSet m_DescriptorSet;
	};
}