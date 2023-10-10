#include "axpch.h"
#include "StorageBuffer.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OGLStorageBuffer.h"

namespace Axton
{
	Ref<StorageBuffer> StorageBuffer::Create(const Specs& specs)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: AX_ASSERT_CORE(false, "RendererAPI::None not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGL::OGLStorageBuffer>(specs);
		}

		AX_ASSERT_CORE(false, "RendererAPI not recognized!");
		return nullptr;
	}
}