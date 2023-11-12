#pragma once

#include "Axton/Renderer/ComputePipeline.h"

namespace Axton::Vulkan
{
	class VKComputePipeline : public ComputePipeline
	{
	public:
		VKComputePipeline(const Specs& specs);

		virtual void Dispatch() override;
	};
}