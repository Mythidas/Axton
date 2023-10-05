#pragma once

#include "RendererAPI.h"
#include "Axton/Core/Defines.h"

namespace Axton
{
	struct Texture3DSpecs
	{
		int Width = 1;
		int Height = 1;
		int Depth = 1;
		ImageFormat Format = ImageFormat::RGBA8;
		FilterFormat Filter = FilterFormat::NEAREST;
		WrapFormat Wrap = WrapFormat::REPEAT;
		bool GenerateMipmaps = false;
	};

	class Texture3D
	{
	public:
		virtual ~Texture3D() = default;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(void* data, int size) = 0;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
		virtual RendererID GetRendererID() const = 0;

		static Ref<Texture3D> Create(const Texture3DSpecs& specs);
		static Ref<Texture3D> Create(const Texture3DSpecs& specs, void* data, int size);
	};
}