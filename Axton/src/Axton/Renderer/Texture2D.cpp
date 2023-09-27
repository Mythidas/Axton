#include "axpch.h"
#include "Texture2D.h"
#include "Platform/OpenGL/Texture2D.h"

namespace Axton
{
	Ref<Texture2D> Texture2D::Create(const Texture2DSpecs& specs)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGL::Texture2D>(specs);
		}

		AX_ASSERT_CORE(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Ref<Texture2D> Texture2D::Create(const Texture2DSpecs& specs, const std::string& path)
	{
		Ref<Texture2D> texture;

		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  texture = CreateRef<OpenGL::Texture2D>(specs); break;
		}

		if (texture)
		{
			texture->SetData(path);
			return texture;
		}

		AX_ASSERT_CORE(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Ref<Texture2D> Texture2D::Create(const Texture2DSpecs& specs, void* data, uint32_t size)
	{
		Ref<Texture2D> texture;

		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  texture = CreateRef<OpenGL::Texture2D>(specs); break;
		}

		if (texture)
		{
			texture->SetData(data, size);
			return texture;
		}

		AX_ASSERT_CORE(false, "Unknown RendererAPI!");
		return nullptr;
	}
}