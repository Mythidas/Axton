#include "axpch.h"
#include "Buffer.h"

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

	OpenGL::VertexBuffer::VertexBuffer(const size_t& size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGL::VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGL::VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGL::VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGL::VertexBuffer::SetLayout(const std::vector<VertexAttrib>& attribs)
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

	void OpenGL::VertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	///////////////// INDEX BUFFER ///////////////////////

	OpenGL::IndexBuffer::IndexBuffer(const uint32_t* indices, const uint32_t count)
	{
		glCreateBuffers(1, &m_RendererID);

		// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
		// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGL::IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGL::IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}
	void OpenGL::IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}