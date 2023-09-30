#include "axpch.h"
#include "OGLVertexArray.h"

#include <glad/glad.h>

namespace Axton::OpenGL
{
	OGLVertexArray::OGLVertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
		glBindVertexArray(m_RendererID);
	}

	OGLVertexArray::~OGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OGLVertexArray::AddBuffers(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer)
	{
		Bind();
		m_VertexBuffer = vertexBuffer;
		m_IndexBuffer = indexBuffer;

		m_VertexBuffer->Bind();
		m_IndexBuffer->Bind();

		Unbind();
	}
}