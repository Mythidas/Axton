#pragma once

#include "Axton/Renderer/RendererAPI.h"
#include "Axton/Renderer/IndexBuffer.h"

namespace Axton::OpenGL
{
	class OGLIndexBuffer : public Axton::IndexBuffer
	{
	public:
		OGLIndexBuffer(const uint32_t* indices, const uint32_t count);
		~OGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		RendererID m_RendererID = 0;
	};
}