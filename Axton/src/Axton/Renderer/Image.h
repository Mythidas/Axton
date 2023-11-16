#pragma once

#include "Axton/Core/Defines.h"
#include "Axton/Math/Vector.h"

namespace Axton
{
	enum class ImageType
	{
		e1D,
		e2D,
		e3D
	};

	enum class ImageUsage
	{
		ShaderRead	= 1 << 0,
		ShaderWrite	= 1 << 1,
		HostRead	= 1 << 2,
		HostWrite	= 1 << 3
	};

	ImageUsage operator|(ImageUsage lhs, ImageUsage rhs);
	bool operator&(ImageUsage lhs, ImageUsage rhs);

	enum class ImageFormat
	{
		R8,
		RGBA8,
		RGBA32F
	};

	enum class ImageTiling
	{
		Linear,
		Nearest
	};

	enum class ImageSamples
	{
		e1
	};

	enum class ImageMipLevels
	{
		e1
	};

	enum class ImageStages
	{
		Graphics	= 1 << 0,
		Compute		= 1 << 1,
		RayTracing	= 1 << 2
	};

	ImageStages operator|(ImageStages lhs, ImageStages rhs);
	bool operator&(ImageStages lhs, ImageStages rhs);

	class Image
	{
	public:
		struct Specs
		{
			uint32_t Binding{ 0 };
			ImageType Type{ ImageType::e2D };
			ImageUsage Usage{};
			ImageFormat Format{};
			ImageTiling Tiling{ ImageTiling::Nearest };
			ImageSamples Samples{ ImageSamples::e1 };
			ImageMipLevels MipLevels{ ImageMipLevels::e1 };
			ImageStages Stages{};
			UVector3 Extents{ 0 };

			Specs& setBinding(uint32_t binding) { Binding = binding; return *this; }
			Specs& setType(ImageType type) { Type = type; return *this; }
			Specs& setUsage(ImageUsage usage) { Usage = usage; return *this; }
			Specs& setFormat(ImageFormat format) { Format = format; return *this; }
			Specs& setTiling(ImageTiling tiling) { Tiling = tiling; return *this; }
			Specs& setSamples(ImageSamples samples) { Samples = samples; return *this; }
			Specs& setMipLevels(ImageMipLevels mipLevels) { MipLevels = mipLevels; return *this; }
			Specs& setStages(ImageStages stages) { Stages = stages; return *this; }
			Specs& setExtents(UVector3 extents) { Extents = extents; return *this; }
			Ref<Image> Build() { return Create(*this); }
		};

		virtual bool Resize(UVector3 extents) = 0;
		virtual void SetData(void* data, size_t size) = 0;

		virtual UVector3 GetExtents() const = 0;
		virtual void* GetRendererID() const = 0;

		static Ref<Image> Create(const Specs& specs);

	public:
		operator bool() const
		{
			return GetRendererID() != nullptr;
		}
	};
}