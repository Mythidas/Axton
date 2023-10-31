#pragma once

#include "RendererAPI.h"

namespace Axton
{
	struct Image1DSpecs
	{
		uint32_t Width = 1;
		uint32_t Slot = 0;
		ImageFormat::Data Format = ImageFormat::Data::RGBA32F;
		ImageFormat::Access Access = ImageFormat::Access::READ_WRITE;
		ImageFormat::Filter Filter = ImageFormat::Filter::LINEAR;
		ImageFormat::Wrap Wrap = ImageFormat::Wrap::REPEAT;
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