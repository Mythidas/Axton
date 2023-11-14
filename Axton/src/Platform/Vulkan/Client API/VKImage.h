#pragma once

#include "Axton/Renderer/Image.h"
#include "../VKImageCore.h"

namespace Axton::Vulkan
{
	class VKImage : public Image
	{
	public:
		VKImage(const Specs& specs);

		virtual void SetData(void* data, size_t size) override;

	private:
		Specs m_Specs;

		Ref<VKImageCore> m_ImageCore;
	};
}