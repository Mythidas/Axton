#include "axpch.h"
#include "Image.h"
#include "Platform/OpenGL/OGLImage.h"

namespace Axton
{
	Ref<Image> Image::Create(const ImageSpecs& specs)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGL::OGLImage>(specs);
		}

		AX_ASSERT_CORE(false, "Unknown RendererAPI!");
		return nullptr;
	}
}