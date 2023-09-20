#include "axpch.h"
#include "Buffer.h"
#include "Axton/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Axton
{
	uint32_t VertexAttrib::GetTypeCount() const
	{
		switch (Type)
		{
		case VertexAttrib::VAType::Float:   return 1;
		case VertexAttrib::VAType::Float2:  return 2;
		case VertexAttrib::VAType::Float3:  return 3;
		case VertexAttrib::VAType::Float4:  return 4;
		case VertexAttrib::VAType::Mat3:    return 3 * 3;
		case VertexAttrib::VAType::Mat4:    return 4 * 4;
		case VertexAttrib::VAType::Int:     return 1;
		case VertexAttrib::VAType::Int2:    return 2;
		case VertexAttrib::VAType::Int3:    return 3;
		case VertexAttrib::VAType::Int4:    return 4;
		case VertexAttrib::VAType::Bool:    return 1;
		}

		AX_ASSERT_CORE(false, "Unknown VertexAttrib::VAType!");
		return 0;
	}

	uint32_t VertexAttrib::GetTypeSize() const
	{
		switch (Type)
		{
		case VertexAttrib::VAType::Float:   return 1 * 4;
		case VertexAttrib::VAType::Float2:  return 2 * 4;
		case VertexAttrib::VAType::Float3:  return 3 * 4;
		case VertexAttrib::VAType::Float4:  return 4 * 4;
		case VertexAttrib::VAType::Mat3:    return 3 * 3 * 4;
		case VertexAttrib::VAType::Mat4:    return 4 * 4 * 4;
		case VertexAttrib::VAType::Int:     return 1 * 4;
		case VertexAttrib::VAType::Int2:    return 2 * 4;
		case VertexAttrib::VAType::Int3:    return 3 * 4;
		case VertexAttrib::VAType::Int4:    return 4 * 4;
		case VertexAttrib::VAType::Bool:    return 1;
		}

		AX_ASSERT_CORE(false, "Unknown VertexAttrib::VAType!");
		return 0;
	}

	Ref<VertexBuffer> VertexBuffer::Create(const size_t& size)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexBuffer>(size);
		}

		AX_ASSERT_CORE(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, const uint32_t count)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLIndexBuffer>(indices, count);
		}

		AX_ASSERT_CORE(false, "Unknown RendererAPI!");
		return nullptr;
	}
}