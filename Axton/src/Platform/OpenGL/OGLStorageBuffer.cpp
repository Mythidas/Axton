#include "axpch.h"
#include "OGLStorageBuffer.h"
#include "OGLUtils.h"

#include <glad/glad.h>

namespace Axton::OpenGL
{
	namespace Utils
	{
		static GLenum BufferUsageToOpenGL(const BufferUsage& usage)
		{
			switch (usage)
			{
			case BufferUsage::DYNAM_DRAW: return GL_DYNAMIC_DRAW;
			case BufferUsage::DYNAM_READ: return GL_DYNAMIC_READ;
			case BufferUsage::DYNAM_COPY: return GL_DYNAMIC_COPY;
			case BufferUsage::STATIC_DRAW: return GL_STATIC_DRAW;
			case BufferUsage::STATIC_READ: return GL_STATIC_READ;
			case BufferUsage::STATIC_COPY: return GL_STATIC_COPY;
			}

			AX_ASSERT_CORE(false, "Uknown BufferUsage!");
			return 0;
		}
	}

	OGLStorageBuffer::OGLStorageBuffer(uint32_t size, BufferUsage usage, uint32_t binding)
		: m_RendererID(0)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, Utils::BufferUsageToOpenGL(usage));
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, m_RendererID);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		OGLUtils::CheckForErrors("Storage Buffer");
	}

	OGLStorageBuffer::~OGLStorageBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OGLStorageBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		if (offset > 0)
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
			glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
		}

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
		memcpy(p, data, size);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}
}