#pragma once

#include "Axton/Renderer/Image.h"
#include "../VKImageCore.h"

namespace Axton::Vulkan
{
	class VKImage : public Image
	{
	public:
		VKImage(const Specs& specs);

		virtual bool Resize(UVector3 extents) override;
		virtual void SetData(void* data, size_t size) override;

		virtual UVector3 GetExtents() const override { return m_Specs.Extents; };
		virtual void* GetRendererID() const override { return m_DescriptorSet; };

		void UpdateDescriptorSet(vk::DescriptorSet& set);

		vk::DescriptorPoolSize GetPoolSize();
		vk::DescriptorSetLayoutBinding GetLayoutBinding();

	private:
		void createImage(UVector3 extents);

	private:
		Specs m_Specs;

		Ref<VKImageCore> m_ImageCore;
		vk::DescriptorSet m_DescriptorSet;
	};
}