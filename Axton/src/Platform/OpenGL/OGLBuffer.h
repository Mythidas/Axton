#pragma once

#include "Axton/Renderer/RendererAPI.h"
#include "Axton/Renderer/Buffer.h"

namespace Axton::OpenGL
{
	class OGLVertexBuffer : public Axton::VertexBuffer
	{
	public:
		OGLVertexBuffer(const size_t& size);
		~OGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetLayout(const std::vector<VertexAttrib>& attribs) override;
		virtual void SetData(const void* data, long long size) override;

	private:
		RendererID m_RendererID;
	};

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