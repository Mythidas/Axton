#pragma once

#include "Axton/Renderer/RendererAPI.h"
#include "Axton/Renderer/VertexArray.h"

namespace Axton
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddBuffers(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer) override;

		virtual Ref<VertexBuffer> GetVertexBuffer() const override { return m_VertexBuffer; }
		virtual Ref<IndexBuffer> GetIndexBuffer() const override { return m_IndexBuffer; }

	private:
		RendererID m_RendererID;

		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}