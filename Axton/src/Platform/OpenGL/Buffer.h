#pragma once

#include "Axton/Renderer/RendererAPI.h"
#include "Axton/Renderer/Buffer.h"

namespace Axton::OpenGL
{
	class VertexBuffer : public Axton::VertexBuffer
	{
	public:
		VertexBuffer(const size_t& size);
		~VertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetLayout(const std::vector<VertexAttrib>& attribs) override;
		virtual void SetData(const void* data, uint32_t size) override;

	private:
		RendererID m_RendererID;
	};

	class IndexBuffer : public Axton::IndexBuffer
	{
	public:
		IndexBuffer(const uint32_t* indices, const uint32_t count);
		~IndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		RendererID m_RendererID = 0;
	};
}