#pragma once

#include "RenderBuffer.h"
#include "PipelineAssets.h"
#include "Axton/Core/Defines.h"

namespace Axton
{
	class ComputePipeline
	{
	public:
		struct Specs
		{
			std::string ShaderPath;
			Ref<PipelineAssets> Assets;

			Specs& setShaderPath(const std::string& path) { ShaderPath = path; return *this; }
			Specs& setAssets(Ref<PipelineAssets> assets) { Assets = assets; return *this; }
			Ref<ComputePipeline> Build() { return Create(*this); }
		};

		virtual void Dispatch(uint32_t width, uint32_t height, uint32_t depth) = 0;

		static Ref<ComputePipeline> Create(const Specs& specs);
	};
}