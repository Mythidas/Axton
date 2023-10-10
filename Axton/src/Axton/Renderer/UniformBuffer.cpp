#include "axpch.h"
#include "UniformBuffer.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OGLUniformBuffer.h"

namespace Axton
{
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding, const std::string& debugName)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None: AX_ASSERT_CORE(false, "RendererAPI::None not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGL::OGLUniformBuffer>(size, binding, debugName);
		}

		AX_ASSERT_CORE(false, "RendererAPI not recognized!");
		return nullptr;
	}
}