#include "axpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Axton
{
	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
		glBindVertexArray(m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddBuffers(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer)
	{
		m_VertexBuffer = vertexBuffer;
		m_IndexBuffer = indexBuffer;
	}
}