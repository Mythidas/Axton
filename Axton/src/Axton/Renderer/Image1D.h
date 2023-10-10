#pragma once

#include "RendererAPI.h"

namespace Axton
{
	struct Image1DSpecs
	{
		uint32_t Width = 1;
		uint32_t Slot = 0;
		AccessFormat Access = AccessFormat::READ_WRITE;
		ImageFormat Format = ImageFormat::RGBA32F;
		FilterFormat Filter = FilterFormat::LINEAR;
		WrapFormat Wrap = WrapFormat::REPEAT;
	};

	class Image1D
	{
	public:
		virtual ~Image1D() = default;

		virtual void Bind() const = 0;
		virtual void SetData(void* data, uint32_t width) = 0;
		virtual void Resize(uint32_t width) = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual RendererID GetRendererID() const = 0;

		static Ref<Image1D> Create(const Image1DSpecs& specs);
	};
}