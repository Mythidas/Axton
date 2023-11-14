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
			ImageType Type;
			ImageUsage Usage;
			ImageFormat Format;
			ImageTiling Tiling;
			ImageSamples Samples;
			ImageMipLevels MipLevels;
			ImageStages Stages;
			UVector3 Extents;

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

		virtual void SetData(void* data, size_t size) = 0;

		static Ref<Image> Create(const Specs& specs);
	};
}