#pragma once

#include "PipelineAssets.h"
#include "Axton/Core/Defines.h"

namespace Axton
{
	class RayTracePipeline
	{
	public:
		struct Specs
		{
			std::string ShaderPath;
			Ref<PipelineAssets> Assets;

			Specs& setShaderPath(const std::string& path) { ShaderPath = path; return *this; }
			Specs& setAssets(Ref<PipelineAssets> assets) { Assets = assets; return *this; }
			Ref<RayTracePipeline> Build() { return Create(*this); }
		};

		static Ref<RayTracePipeline> Create(const Specs& specs);
	};
}