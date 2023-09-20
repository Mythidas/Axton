#include "axpch.h"
#include "Texture2D.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace Axton
{
	Ref<Texture2D> Texture2D::Create(const Texture2DSpecs& specs)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(specs);
		}

		AX_ASSERT_CORE(false, "Unknown RendererAPI!");
		return nullptr;
	}
}