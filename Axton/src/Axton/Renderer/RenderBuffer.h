#pragma once

#include "Axton/Core/Defines.h"

namespace Axton
{
	enum BufferUsage
	{
		Vertex,
		Index,
		Uniform,
		ShaderStorage
	};

	class RenderBuffer
	{
	public:
		struct Specs
		{
			size_t Size;
			BufferUsage Usage;

			Specs& setSize(size_t size) { Size = size; return *this; }
			Specs& setUsage(BufferUsage usage) { Usage = usage; return *this; }
			Ref<RenderBuffer> Build() { return Create(*this); }
		};

		virtual void Bind() const = 0;
		virtual void SetData(void* data, size_t size) = 0;

		virtual BufferUsage GetUsage() const = 0;

		static Ref<RenderBuffer> Create(const Specs& specs);
	};
}