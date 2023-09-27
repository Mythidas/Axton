#include "axpch.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/RendererAPI.h"

namespace Axton
{
	RendererAPI::API RendererAPI::s_API = API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGL::RendererAPI>();
		}

		AX_ASSERT_CORE(false, "Uknown RendererAPI!");
		return nullptr;
	}
}