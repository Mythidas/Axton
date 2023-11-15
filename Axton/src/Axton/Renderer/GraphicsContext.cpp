#include "axpch.h"
#include "GraphicsContext.h"
#include "Axton/Core/Assert.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OGLGraphicsContext.h"
#include "Platform/Vulkan/VKGraphicsContext.h"

namespace Axton
{
	Scope<GraphicsContext> GraphicsContext::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateScope<OpenGL::OGLGraphicsContext>();
			case RendererAPI::API::Vulkan:  return CreateScope<Vulkan::VKGraphicsContext>();
		}

		AX_ASSERT_CORE(false, "Unknown RendererAPI!");
		return nullptr;
	}
}