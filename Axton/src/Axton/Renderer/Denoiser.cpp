#include "axpch.h"
#include "Denoiser.h"
#include "RenderEngine.h"
#include "Platform/Vulkan/Client API/VKDenoiser.h"

namespace Axton
{
	Ref<Denoiser> Denoiser::Create(const Specs& specs)
	{
		switch (RenderEngine::GetAPI())
		{
		case RenderAPI::None: AX_ASSERT_CORE(false, "API::None currently unsupported!"); return nullptr;
		case RenderAPI::Vulkan: return CreateRef<Vulkan::VKDenoiser>(specs);
		}

		AX_ASSERT_CORE(false, "Unknown RenderEngine::API! (Denoiser)");
		return nullptr;
	}
}