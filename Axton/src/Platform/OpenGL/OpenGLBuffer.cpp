#include "axpch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Axton
{
	///////////////// VERTEX BUFFER ///////////////////////

	namespace Utils
	{
		static GLenum ConvertVertexTypeToGL(const VertexAttrib::VAType& type)
		{
			switch (type)
			{
				case VertexAttrib::VAType::Float:	return GL_FLOAT;
				case VertexAttrib::VAType::Float2:	return GL_FLOAT;
				case VertexAttrib::VAType::Float3:	return GL_FLOAT;
				case VertexAttrib::VAType::Float4:	return GL_FLOAT;
				case VertexAttrib::VAType::Mat3:	return GL_FLOAT;
				case VertexAttrib::VAType::Mat4:	return GL_FLOAT;
				case VertexAttrib::VAType::Int:		return GL_INT;
				case VertexAttrib::VAType::Int2:	return GL_INT;
				case VertexAttrib::VAType::Int3:	return GL_INT;
				case VertexAttrib::VAType::Int4:	return GL_INT;
				case VertexAttrib::VAType::Bool:	return GL_BOOL;
			}

			AX_ASSERT_CORE(false, "Unknown VertexAttrib::VAType!");
			return 0;
		}
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(const size_t& size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		
		// TODO: Add a check to see if the buffer is dynamic or not
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetLayout(const std::vector<VertexAttrib>& attribs)
	{
		size_t stride = 0;
		for (const auto& attrib : attribs)
		{
			stride += attrib.GetTypeSize();
		}

		size_t index = 0;
		size_t offset = 0;
		for (const auto& attrib : attribs)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, attrib.GetTypeCount(), Utils::ConvertVertexTypeToGL(attrib.Type),
				attrib.Normalized ? GL_TRUE : GL_FALSE, stride,
				(void*)offset);

			offset += attrib.GetTypeSize();
			index++;
		}
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	///////////////// INDEX BUFFER ///////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* indices, const uint32_t count)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		// TODO: Add a check to see if the buffer is dynamic or not
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_DYNAMIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}
	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}