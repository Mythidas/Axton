#pragma once

#include "Axton/Core/Defines.h"
#include "Axton/Math/Vector.h"

namespace Axton
{
	enum class TextureType
	{
		e1D,
		e2D,
		e3D
	};

	enum class TextureUsage
	{
		ShaderRead	= 1 << 0,
		ShaderWrite	= 1 << 1,
		HostRead	= 1 << 2,
		HostWrite	= 1 << 3
	};

	TextureUsage operator|(TextureUsage lhs, TextureUsage rhs);
	bool operator&(TextureUsage lhs, TextureUsage rhs);

	enum class TextureFormat
	{
		R8,
		RGBA8,
		RG32F,
		RGBA32F
	};

	enum class TextureTiling
	{
		Linear,
		Nearest
	};

	enum class TextureSamples
	{
		e1
	};

	enum class TextureMipLevels
	{
		e1
	};

	enum class TextureStages
	{
		Graphics	= 1 << 0,
		Compute		= 1 << 1,
		RayTracing	= 1 << 2
	};

	TextureStages operator|(TextureStages lhs, TextureStages rhs);
	bool operator&(TextureStages lhs, TextureStages rhs);

	class Texture
	{
	public:
		struct Specs
		{
			uint32_t Binding{ 0 };
			TextureType Type{ TextureType::e2D };
			TextureUsage Usage{};
			TextureFormat Format{};
			TextureTiling Tiling{ TextureTiling::Nearest };
			TextureSamples Samples{ TextureSamples::e1 };
			TextureMipLevels MipLevels{ TextureMipLevels::e1 };
			TextureStages Stages{};
			UVector3 Extents{ 0 };

			Specs& setBinding(uint32_t binding) { Binding = binding; return *this; }
			Specs& setType(TextureType type) { Type = type; return *this; }
			Specs& setUsage(TextureUsage usage) { Usage = usage; return *this; }
			Specs& setFormat(TextureFormat format) { Format = format; return *this; }
			Specs& setTiling(TextureTiling tiling) { Tiling = tiling; return *this; }
			Specs& setSamples(TextureSamples samples) { Samples = samples; return *this; }
			Specs& setMipLevels(TextureMipLevels mipLevels) { MipLevels = mipLevels; return *this; }
			Specs& setStages(TextureStages stages) { Stages = stages; return *this; }
			Specs& setExtents(UVector3 extents) { Extents = extents; return *this; }
			Ref<Texture> Build() { return Create(*this); }
		};

		virtual bool Resize(UVector3 extents) = 0;
		virtual void SetData(void* data, size_t size) = 0;

		virtual UVector3 GetExtents() const = 0;
		virtual void* GetRendererID() const = 0;

		static Ref<Texture> Create(const Specs& specs);

	public:
		operator bool() const
		{
			return GetRendererID() != nullptr;
		}
	};
}