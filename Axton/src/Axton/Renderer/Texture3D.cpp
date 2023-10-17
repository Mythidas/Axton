#include "axpch.h"
#include "Texture3D.h"
#include "Platform/OpenGL/OGLTexture3D.h"

namespace Axton
{
	Ref<Texture3D> Texture3D::Create(const Texture3DSpecs& specs)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGL::OGLTexture3D>(specs);
		}

		AX_ASSERT_CORE(false, "Unknown RendererAPI!");
		return nullptr;
	}
}