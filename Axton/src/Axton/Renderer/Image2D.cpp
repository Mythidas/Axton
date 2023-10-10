#include "axpch.h"
#include "Image2D.h"
#include "Platform/OpenGL/OGLImage2D.h"

namespace Axton
{
	Ref<Image2D> Image2D::Create(const Image2DSpecs& specs)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGL::OGLImage2D>(specs);
		}

		AX_ASSERT_CORE(false, "Unknown RendererAPI!");
		return nullptr;
	}
}