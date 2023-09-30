#pragma once

#include "Axton/Renderer/UniformBuffer.h"
#include "Axton/Renderer/RendererAPI.h"

namespace Axton::OpenGL
{
	class OGLUniformBuffer : public Axton::UniformBuffer
	{
	public:
		OGLUniformBuffer(uint32_t size, uint32_t binding);
		~OGLUniformBuffer();

		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

	private:
		RendererID m_RendererID = 0;
	};
}