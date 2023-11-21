#pragma once

#include "Axton/Renderer/Denoiser.h"

namespace Axton::Vulkan
{
	class VKDenoiser : public Denoiser
	{
	public:
		VKDenoiser(const Specs& specs);
		~VKDenoiser();
	};
}