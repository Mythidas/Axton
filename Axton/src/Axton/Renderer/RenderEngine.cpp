#include "axpch.h"
#include "RenderEngine.h"
#include "Platform/Vulkan/VKRenderEngine.h"

namespace Axton
{
	GraphicsAPI RenderEngine::s_API = GraphicsAPI::Vulkan;

	Scope<RenderEngine> RenderEngine::Create(void* windowHandle, const Specs& specs)
	{
		s_API = specs.API;

		switch (s_API)
		{
		case GraphicsAPI::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); break;
		case GraphicsAPI::Vulkan:  return CreateScope<Vulkan::VKRenderEngine>(windowHandle, specs); break;
		}

		AX_ASSERT_CORE(false, "Uknown Renderer::API!");
		return nullptr;
	}
}