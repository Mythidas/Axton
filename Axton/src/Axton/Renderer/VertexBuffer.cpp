#include "axpch.h"
#include "VertexBuffer.h"
#include "Axton/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OGLVertexBuffer.h"

namespace Axton
{
	uint32_t Axton::VertexBuffer::Attribute::GetTypeCount() const
	{
		switch (Format)
		{
		case VertexBuffer::Attribute::AttributeFormat::Float:   return 1;
		case VertexBuffer::Attribute::AttributeFormat::Float2:  return 2;
		case VertexBuffer::Attribute::AttributeFormat::Float3:  return 3;
		case VertexBuffer::Attribute::AttributeFormat::Float4:  return 4;
		case VertexBuffer::Attribute::AttributeFormat::Mat3:    return 3 * 3;
		case VertexBuffer::Attribute::AttributeFormat::Mat4:    return 4 * 4;
		case VertexBuffer::Attribute::AttributeFormat::Int:     return 1;
		case VertexBuffer::Attribute::AttributeFormat::Int2:    return 2;
		case VertexBuffer::Attribute::AttributeFormat::Int3:    return 3;
		case VertexBuffer::Attribute::AttributeFormat::Int4:    return 4;
		case VertexBuffer::Attribute::AttributeFormat::Bool:    return 1;
		}

		AX_ASSERT_CORE(false, "Unknown VertexBuffer Attribute::Format!");
		return 0;
	}

	uint32_t Axton::VertexBuffer::Attribute::GetTypeSize() const
	{
		switch (Format)
		{
		case VertexBuffer::Attribute::AttributeFormat::Float:   return 1 * 4;
		case VertexBuffer::Attribute::AttributeFormat::Float2:  return 2 * 4;
		case VertexBuffer::Attribute::AttributeFormat::Float3:  return 3 * 4;
		case VertexBuffer::Attribute::AttributeFormat::Float4:  return 4 * 4;
		case VertexBuffer::Attribute::AttributeFormat::Mat3:    return 3 * 3 * 4;
		case VertexBuffer::Attribute::AttributeFormat::Mat4:    return 4 * 4 * 4;
		case VertexBuffer::Attribute::AttributeFormat::Int:     return 1 * 4;
		case VertexBuffer::Attribute::AttributeFormat::Int2:    return 2 * 4;
		case VertexBuffer::Attribute::AttributeFormat::Int3:    return 3 * 4;
		case VertexBuffer::Attribute::AttributeFormat::Int4:    return 4 * 4;
		case VertexBuffer::Attribute::AttributeFormat::Bool:    return 1 * 1;
		}

		AX_ASSERT_CORE(false, "Unknown VertexBuffer Attribute::AttributeFormat!");
		return 0;
	}

	Ref<VertexBuffer> VertexBuffer::Create(const size_t& size)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGL::OGLVertexBuffer>(size);
		}

		AX_ASSERT_CORE(false, "Unknown RendererAPI!");
		return nullptr;
	}
}