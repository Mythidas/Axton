#include "axpch.h"
#include "OGLUniformBuffer.h"
#include "OGLUtils.h"
#include "Axton/Debug/MemTracker.h"

#include <glad/glad.h>

namespace Axton::OpenGL
{
	OGLUniformBuffer::OGLUniformBuffer(uint32_t size, uint32_t binding, const std::string& debugName)
		: m_RendererID(0), m_DebugName(debugName)
	{
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);

		OGLUtils::CheckForErrors("Uniform Buffer");

		if (!debugName.empty())
			MemTracker::SetSlot(debugName, size);
	}

	OGLUniformBuffer::~OGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);

		if (!m_DebugName.empty())
			MemTracker::DeleteSlot(m_DebugName);
	}

	void OGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		glNamedBufferSubData(m_RendererID, offset, size, data);
	}
}