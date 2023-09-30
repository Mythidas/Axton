#include "axpch.h"
#include "Framebuffer.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OGLFramebuffer.h"

namespace Axton
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpec& specs)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None: AX_ASSERT_CORE(false, "RendererAPI::None not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGL::OGLFramebuffer>(specs);
		}

		AX_ASSERT_CORE(false, "RendererAPI not recognized!");
		return nullptr;
	}
}