#include "axpch.h"
#include "Image1D.h"
#include "Platform/OpenGL/OGLImage1D.h"

namespace Axton
{
	Ref<Image1D> Image1D::Create(const Image1DSpecs& specs)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGL::OGLImage1D>(specs);
		}

		AX_ASSERT_CORE(false, "Unknown RendererAPI!");
		return nullptr;
	}
}