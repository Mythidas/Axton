#include "axpch.h"
#include "StorageBuffer.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OGLStorageBuffer.h"

namespace Axton
{
	Ref<StorageBuffer> StorageBuffer::Create(uint32_t size, BufferUsage usage, uint32_t binding)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: AX_ASSERT_CORE(false, "RendererAPI::None not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGL::OGLStorageBuffer>(size, usage, binding);
		}

		AX_ASSERT_CORE(false, "RendererAPI not recognized!");
		return nullptr;
	}
}