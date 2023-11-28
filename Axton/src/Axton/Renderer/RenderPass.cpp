#include "axpch.h"
#include "RenderPass.h"
#include "RenderEngine.h"
#include "Platform/Vulkan/Client API/VKRenderPass.h"

namespace Axton
{
	Ref<RenderPass> RenderPass::Create(const Specs& specs)
	{
		switch (RenderEngine::GetAPI())
		{
		case RenderAPI::None: AssertCore(false, "RenderAPI::None currently unsupported!"); return nullptr;
		case RenderAPI::Vulkan: return CreateRef<Vulkan::VKRenderPass>(specs);
		}

		AssertCore(false, "Unknown RenderAPI (RenderPass)");
		return nullptr;
	}
}