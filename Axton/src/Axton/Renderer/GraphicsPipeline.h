#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RenderBuffer.h"
#include "Axton/Core/Defines.h"

namespace Axton
{
	enum VertexAttribute
	{
		Float2,
		Float3,
		Float4
	};

	class GraphicsPipeline
	{
	public:
		struct Specs
		{
			std::string VertPath;
			std::string FragPath;
			Ref<VertexBuffer> VertexBuffer;
			Ref<IndexBuffer> IndexBuffer;
			std::vector<Ref<RenderBuffer>> Buffers;
			std::vector<VertexAttribute> VertexAttributes;

			Specs& setVertPath(std::string path) { VertPath = path; return *this; }
			Specs& setFragPath(std::string path) { FragPath = path; return *this; }
			Specs& setVertexBuffer(Ref<Axton::VertexBuffer> buffer) { VertexBuffer = buffer; return *this; }
			Specs& setIndexBuffer(Ref<Axton::IndexBuffer> buffer) { IndexBuffer = buffer; return *this; }
			Specs& setBuffers(const std::vector<Ref<RenderBuffer>>& buffers) { Buffers = buffers; return *this; }
			Specs& setVertexAttributes(const std::vector<VertexAttribute>& attributes) { VertexAttributes = attributes; return *this; }
			Ref<GraphicsPipeline> Build() { return Create(*this); }
		};

		virtual void Render(uint32_t count) = 0;

		static Ref<GraphicsPipeline> Create(const Specs& specs);
	};
}