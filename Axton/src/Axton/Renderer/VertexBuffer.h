#pragma once

#include "Axton/Core/Defines.h"
#include "RenderBuffer.h"

namespace Axton
{
	class VertexBuffer
	{
	public:
		struct Specs
		{
			size_t Size{ 0 };
			BufferRate Rate{ BufferRate::Static };

			Specs& setSize(size_t size) { Size = size; return *this; }
			Specs& setRate(BufferRate rate) { Rate = rate; return *this; }
			Ref<VertexBuffer> Build() { return Create(*this); }
		};

		virtual void Bind() const = 0;
		virtual void SetData(void* data, size_t size, uint32_t offset) = 0;

		static Ref<VertexBuffer> Create(const VertexBuffer::Specs& specs);
	};
}