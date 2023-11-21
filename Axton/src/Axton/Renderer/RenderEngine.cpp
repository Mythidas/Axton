#include "axpch.h"
#include "RenderEngine.h"
#include "Platform/Vulkan/VKRenderEngine.h"

namespace Axton
{
	RenderAPI RenderEngine::s_API = RenderAPI::Vulkan;

	Scope<RenderEngine> RenderEngine::Create(void* windowHandle, const Specs& specs)
	{
		s_API = specs.API;

		switch (s_API)
		{
		case RenderAPI::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); break;
		case RenderAPI::Vulkan:  return CreateScope<Vulkan::VKRenderEngine>(windowHandle, specs); break;
		}

		AX_ASSERT_CORE(false, "Uknown Renderer::API!");
		return nullptr;
	}

	RenderExtension operator|(RenderExtension lhs, RenderExtension rhs)
	{
		return static_cast<RenderExtension>(int(lhs) | int(rhs));
	}

	bool operator&(RenderExtension lhs, RenderExtension rhs)
	{
		return static_cast<bool>(int(lhs) & int(rhs));
	}
}