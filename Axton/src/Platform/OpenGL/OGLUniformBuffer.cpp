#include "axpch.h"
#include "OGLUniformBuffer.h"

#include <glad/glad.h>

namespace Axton::OpenGL
{
	OGLUniformBuffer::OGLUniformBuffer(uint32_t size, uint32_t binding)
	{
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
	}

	OGLUniformBuffer::~OGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		glNamedBufferSubData(m_RendererID, offset, size, data);
	}
}