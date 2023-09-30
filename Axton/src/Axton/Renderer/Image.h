#pragma once

#include "RendererAPI.h"

namespace Axton
{
	struct ImageSpecs
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		ImageFormat Format = ImageFormat::RGBA32F;
		FilterFormat Filter = FilterFormat::LINEAR;
		WrapFormat Wrap = WrapFormat::REPEAT;

		ImageSpecs(uint32_t width, uint32_t height) : Width(width), Height(height) {}
	};

	class Image
	{
	public:
		virtual ~Image() = default;

		virtual void Bind() const = 0;
		virtual void SetData(void* data) const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual RendererID GetRendererID() const = 0;

		static Ref<Image> Create(const ImageSpecs& specs);
	};
}