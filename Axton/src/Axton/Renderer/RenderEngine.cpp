#include "axpch.h"
#include "RenderEngine.h"
#include "Platform/Vulkan/VKRenderEngine.h"

namespace Axton
{
	RenderEngine::API RenderEngine::s_API = API::Vulkan;

	Scope<RenderEngine> RenderEngine::Create(void* windowHandle)
	{
		switch (s_API)
		{
		case RenderEngine::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); break;
		case RenderEngine::API::Vulkan:  return CreateScope<Vulkan::VKRenderEngine>(windowHandle); break;
		}

		AX_ASSERT_CORE(false, "Uknown Renderer::API!");
		return nullptr;
	}
}