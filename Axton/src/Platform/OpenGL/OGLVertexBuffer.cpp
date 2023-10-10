#include "axpch.h"
#include "OGLVertexBuffer.h"

#include <glad/glad.h>

namespace Axton::OpenGL
{
	namespace Utils
	{
		static GLenum ConverteVertexAttributeFormat(VertexBuffer::Attribute::AttributeFormat format)
		{
			switch (format)
			{
			case VertexBuffer::Attribute::AttributeFormat::Float:	return GL_FLOAT;
			case VertexBuffer::Attribute::AttributeFormat::Float2:	return GL_FLOAT;
			case VertexBuffer::Attribute::AttributeFormat::Float3:	return GL_FLOAT;
			case VertexBuffer::Attribute::AttributeFormat::Float4:	return GL_FLOAT;
			case VertexBuffer::Attribute::AttributeFormat::Mat3:	return GL_FLOAT;
			case VertexBuffer::Attribute::AttributeFormat::Mat4:	return GL_FLOAT;
			case VertexBuffer::Attribute::AttributeFormat::Int:		return GL_INT;
			case VertexBuffer::Attribute::AttributeFormat::Int2:	return GL_INT;
			case VertexBuffer::Attribute::AttributeFormat::Int3:	return GL_INT;
			case VertexBuffer::Attribute::AttributeFormat::Int4:	return GL_INT;
			case VertexBuffer::Attribute::AttributeFormat::Bool:	return GL_BOOL;
			}

			AX_ASSERT_CORE(false, "Unknown VertexBuffer Attribute::Format!");
			return 0;
		}
	}

	OGLVertexBuffer::OGLVertexBuffer(const size_t& size)
		: m_RendererID(0)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OGLVertexBuffer::~OGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OGLVertexBuffer::SetLayout(const std::vector<VertexBuffer::Attribute>& attribs)
	{
		GLsizei stride = 0;
		for (const auto& attrib : attribs)
		{
			stride += attrib.GetTypeSize();
		}

		GLuint index = 0;
		size_t offset = 0;
		for (const auto& attrib : attribs)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, attrib.GetTypeCount(), Utils::ConverteVertexAttributeFormat(attrib.Format),
				attrib.Normalized ? GL_TRUE : GL_FALSE, stride,
				(void*)offset);

			offset += attrib.GetTypeSize();
			index++;
		}
	}

	void OGLVertexBuffer::SetData(const void* data, long long size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}
}