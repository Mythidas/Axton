#pragma once

#include "RendererAPI.h"
#include "Axton/Core/Defines.h"

namespace Axton
{
	enum class ImageFormat
	{
		None = 0,
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};

	enum class FilterFormat
	{
		NEAREST,
		LINEAR
	};

	enum class WrapFormat
	{
		CLAMP,
		REPEAT
	};

	struct Texture2DSpecs
	{
		int Width = 1;
		int Height = 1;
		ImageFormat Format = ImageFormat::RGBA8;
		FilterFormat Filter = FilterFormat::NEAREST;
		WrapFormat Wrap = WrapFormat::REPEAT;
		bool GenerateMipmaps = true;
	};

	class AX_API Texture2D
	{
	public:
		virtual ~Texture2D() = default;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;
		virtual void SetData(const std::string& path) = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual RendererID GetRendererID() const = 0;

		static Ref<Texture2D> Create(const Texture2DSpecs& specs);
		static Ref<Texture2D> Create(const Texture2DSpecs& specs, const std::string& path);
		static Ref<Texture2D> Create(const Texture2DSpecs& specs, void*data, uint32_t size);
	};
}