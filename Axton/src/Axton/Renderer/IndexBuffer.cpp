#include "axpch.h"
#include "IndexBuffer.h"
#include "Axton/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OGLIndexBuffer.h"

namespace Axton
{
	Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, const uint32_t count)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGL::OGLIndexBuffer>(indices, count);
		}

		AX_ASSERT_CORE(false, "Unknown RendererAPI!");
		return nullptr;
	}
}