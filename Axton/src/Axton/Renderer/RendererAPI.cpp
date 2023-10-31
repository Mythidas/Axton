#include "axpch.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OGLRendererAPI.h"
#include "Platform/Vulkan/VKRendererAPI.h"

namespace Axton
{
	RendererAPI::API RendererAPI::s_API = API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGL::OGLRendererAPI>();
		case RendererAPI::API::Vulkan:  return CreateScope<Vulkan::VKRendererAPI>();
		}

		AX_ASSERT_CORE(false, "Uknown RendererAPI!");
		return nullptr;
	}
}