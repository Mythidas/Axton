#pragma once

#include "Axton/Renderer/RendererAPI.h"
#include "Axton/Renderer/Buffer.h"

namespace Axton
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const size_t& size);
		~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetLayout(const std::vector<VertexAttrib>& attribs) override;
		virtual void SetData(const void* data, uint32_t size) override;

	private:
		RendererID m_RendererID;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const uint32_t* indices, const uint32_t count);
		~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		RendererID m_RendererID;
	};
}