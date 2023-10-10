#include "axpch.h"
#include "OGLIndexBuffer.h"

#include <glad/glad.h>

namespace Axton::OpenGL
{
	OGLIndexBuffer::OGLIndexBuffer(const uint32_t* indices, const uint32_t count)
	{
		glCreateBuffers(1, &m_RendererID);

		// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
		// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OGLIndexBuffer::~OGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}
	void OGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}