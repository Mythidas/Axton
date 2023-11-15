#pragma once

#include "Axton/Core/Defines.h"
#include "RenderBuffer.h"
#include "Image.h"

namespace Axton
{
	class PipelineAssets
	{
	public:
		struct Specs
		{
			std::vector<Ref<RenderBuffer>> Buffers;
			std::vector<Ref<Image>> Images;

			Specs& setBuffers(const std::vector<Ref<RenderBuffer>>& buffers) { Buffers = buffers; return *this; };
			Specs& setImages(const std::vector<Ref<Image>>& images) { Images = images; return *this; }
			Ref<PipelineAssets> Build() { return Create(*this); }
		};

		virtual bool Empty() const = 0;

		static Ref<PipelineAssets> Create(const PipelineAssets::Specs& specs);
	};
}