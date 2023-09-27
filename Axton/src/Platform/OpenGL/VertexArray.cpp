#include "axpch.h"
#include "VertexArray.h"

#include <glad/glad.h>

namespace Axton
{
	OpenGL::VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
		glBindVertexArray(m_RendererID);
	}

	OpenGL::VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGL::VertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGL::VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGL::VertexArray::AddBuffers(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer)
	{
		Bind();
		m_VertexBuffer = vertexBuffer;
		m_IndexBuffer = indexBuffer;

		m_VertexBuffer->Bind();
		m_IndexBuffer->Bind();

		Unbind();
	}
}