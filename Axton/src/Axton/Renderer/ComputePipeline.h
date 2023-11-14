#pragma once

#include "RenderBuffer.h"
#include "Axton/Core/Defines.h"

namespace Axton
{
	class ComputePipeline
	{
	public:
		struct Specs
		{
			std::string ShaderPath;
			std::vector<Ref<RenderBuffer>> Buffers;

			Specs& setShaderPath(const std::string& path) { ShaderPath = path; return *this; }
			Specs& setBuffers(const std::vector<Ref<RenderBuffer>>& buffers) { Buffers = buffers; return *this; }
			Ref<ComputePipeline> Build() { return Create(*this); }
		};

		virtual void Dispatch(uint32_t width, uint32_t height, uint32_t depth) = 0;

		static Ref<ComputePipeline> Create(const Specs& specs);
	};
}