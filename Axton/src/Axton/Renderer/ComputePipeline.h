#pragma once

#include "Axton/Core/Defines.h"

namespace Axton
{
	class ComputePipeline
	{
	public:
		struct Specs
		{

		};

		virtual void Dispatch() = 0;

		static Ref<ComputePipeline> Create(const Specs& specs);
	};
}