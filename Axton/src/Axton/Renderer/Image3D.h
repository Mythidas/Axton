#pragma once

#include "RendererAPI.h"

namespace Axton
{
	struct Image3DSpecs
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		uint32_t Depth = 1;
		uint32_t Slot = 0;
		AccessFormat Access = AccessFormat::READ_WRITE;
		ImageFormat Format = ImageFormat::RGBA32F;
		FilterFormat Filter = FilterFormat::LINEAR;
		WrapFormat Wrap = WrapFormat::REPEAT;
	};

	class Image3D
	{
	public:
		virtual ~Image3D() = default;

		virtual void Bind() const = 0;
		virtual void SetData(void* data) = 0;
		virtual void SetData(void* data, uint32_t width, uint32_t height, uint32_t depth) = 0;
		virtual void SetSubData(void* data, IVector3 offset, IVector3 size) = 0;
		virtual void SetSubData(void* data, IVector3 offset, IVector3 size, uint32_t width, uint32_t height, uint32_t depth) = 0;
		virtual void Resize(uint32_t width, uint32_t height, uint32_t depth) = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual RendererID GetRendererID() const = 0;

		static int MaxSize;

		static void BindAll(uint32_t slot, const std::vector <RendererID>& images);
		static Ref<Image3D> Create(const Image3DSpecs& specs);
	};
}