#pragma once

#include "RendererAPI.h"

namespace Axton
{
	struct Image2DSpecs
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		uint32_t Slot = 0;
		ImageFormat::Data Format = ImageFormat::Data::RGBA32F;
		ImageFormat::Access Access = ImageFormat::Access::READ_WRITE;
		ImageFormat::Filter Filter = ImageFormat::Filter::LINEAR;
		ImageFormat::Wrap Wrap = ImageFormat::Wrap::REPEAT;
	};

	class Image2D
	{
	public:
		virtual ~Image2D() = default;

		virtual void Bind() const = 0;
		virtual void SetData(void* data, uint32_t width, uint32_t height) = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual RendererID GetRendererID() const = 0;

		static Ref<Image2D> Create(const Image2DSpecs& specs);
	};
}