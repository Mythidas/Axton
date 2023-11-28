#pragma once

#include "Axton/Core/Defines.h"

namespace Axton
{
	class RenderPass
	{
	public:
		struct Specs
		{
			Ref<RenderPass> Build() { return Create(*this); }
		};

		static Ref<RenderPass> Create(const Specs& specs);
	};
}