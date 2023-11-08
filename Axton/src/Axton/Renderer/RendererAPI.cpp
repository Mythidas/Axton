#include "axpch.h"
#include "RendererAPI.h"
#include "Platform/Vulkan/VKRendererAPI.h"

namespace Axton
{
	RendererAPI::API RendererAPI::s_API = API::Vulkan;

	Scope<RendererAPI> RendererAPI::Create(void* windowHandle)
	{
		switch (s_API)
		{
		case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); break;
		case RendererAPI::API::Vulkan:  return CreateScope<Vulkan::VKRendererAPI>(windowHandle); break;
		}

		AX_ASSERT_CORE(false, "Uknown Renderer::API!");
		return nullptr;
	}
}