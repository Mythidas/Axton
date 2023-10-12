#include "axpch.h"
#include "OGLStorageBuffer.h"
#include "OGLUtils.h"
#include "Axton/Debug/MemTracker.h"

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

	OGLStorageBuffer::OGLStorageBuffer(const StorageBuffer::Specs& specs)
		: m_RendererID(0), m_DebugName(specs.DebugName)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, specs.Size, nullptr, Utils::BufferUsageToOpenGL(specs.Usage));
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, specs.Binding, m_RendererID);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		OGLUtils::CheckForErrors("Storage Buffer");

		if (!m_DebugName.empty())
			MemTracker::SetSlot(m_DebugName, specs.Size);
	}

	OGLStorageBuffer::~OGLStorageBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);

		if (!m_DebugName.empty())
			MemTracker::DeleteSlot(m_DebugName);
	}

	void* OGLStorageBuffer::MapBufferPtr() const
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		return glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
	}

	void OGLStorageBuffer::UnmapBufferPtr() const
	{
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}

	void OGLStorageBuffer::SetData(const void* data, size_t size, uint32_t offset)
	{
		if (offset > 0)
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
			glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
			return;
		}

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
		memcpy(p, data, size);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}
}