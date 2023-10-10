#pragma once

#include "Axton/Renderer/RendererAPI.h"
#include "Axton/Renderer/VertexBuffer.h"

namespace Axton::OpenGL
{
	class OGLVertexBuffer : public VertexBuffer
	{
	public:
		OGLVertexBuffer(const size_t& size);
		~OGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetLayout(const std::vector<VertexBuffer::Attribute>& attribs) override;
		virtual void SetData(const void* data, long long size) override;

	private:
		RendererID m_RendererID;
	};
}