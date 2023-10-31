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
		ImageFormat::Data Format = ImageFormat::Data::RGBA32F;
		ImageFormat::Filter Filter = ImageFormat::Filter::LINEAR;
		ImageFormat::Wrap Wrap = ImageFormat::Wrap::REPEAT;
		bool GenerateMipmaps = false;
	};

	class Texture3D
	{
	public:
		virtual ~Texture3D() = default;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(void* data, IVector3 offset) = 0;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
		virtual RendererID GetRendererID() const = 0;

		static Ref<Texture3D> Create(const Texture3DSpecs& specs);
	};
}