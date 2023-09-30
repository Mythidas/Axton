#pragma once

#include "RendererAPI.h"

namespace Axton
{
	class ComputeShader
	{
	public:
		virtual ~ComputeShader() = default;

		virtual void Dispatch(uint32_t width, uint32_t height, uint32_t depth) const = 0;
		virtual void Barrier() const = 0;

		static Ref<ComputeShader> Create(const std::string& srcPath);
	};
}