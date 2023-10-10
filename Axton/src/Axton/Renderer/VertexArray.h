#pragma once

#include "Axton/Core/Defines.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <vector>

namespace Axton
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddBuffers(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer) = 0;

		virtual Ref<VertexBuffer> GetVertexBuffer() const = 0;
		virtual Ref<IndexBuffer> GetIndexBuffer() const = 0;

		static Ref<VertexArray> Create();
	};
}