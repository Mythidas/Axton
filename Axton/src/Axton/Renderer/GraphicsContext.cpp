#include "axpch.h"
#include "GraphicsContext.h"
#include "Axton/Core/Assert.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLGraphicsContext.h"

namespace Axton
{
	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateScope<OpenGLGraphicsContext>(static_cast<GLFWwindow*>(window));
		}

		AX_ASSERT_CORE(false, "Unknown RendererAPI!");
		return nullptr;
	}
}