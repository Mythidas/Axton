#include "axpch.h"
#include "Image3D.h"
#include "Platform/OpenGL/OGLImage3D.h"

namespace Axton
{
	void Image3D::BindAll(uint32_t slot, const std::vector<RendererID>& images)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return;
		case RendererAPI::API::OpenGL:  OpenGL::OGLImage3D::BindAll(slot, images); return;
		}

		AX_ASSERT_CORE(false, "Unknown RendererAPI!");
	}

	Ref<Image3D> Image3D::Create(const Image3DSpecs& specs)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGL::OGLImage3D>(specs);
		}

		AX_ASSERT_CORE(false, "Unknown RendererAPI!");
		return nullptr;
	}
}