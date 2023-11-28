#include "axpch.h"
#include "Swapchain.h"
#include "RenderEngine.h"
#include "Platform/Vulkan/Client API/VKSwapchain.h"

namespace Axton
{
	Ref<Swapchain> Swapchain::Create(const Specs& specs)
	{
		switch (RenderEngine::GetAPI())
		{
		case RenderAPI::None: AssertCore(false, "RenderAPI::None currently unsupported!"); return nullptr;
		case RenderAPI::Vulkan: return CreateRef<Vulkan::VKSwapchain>(specs);
		}

		AssertCore(false, "Unknown RenderAPI (Swapchain)");
		return nullptr;
	}
}