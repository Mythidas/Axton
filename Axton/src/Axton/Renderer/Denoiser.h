#pragma once

#include "Axton/Core/Defines.h"

namespace Axton
{
	class Denoiser
	{
	public:
		struct Specs
		{

		};

		static Ref<Denoiser> Create(const Specs& specs);
	};
}