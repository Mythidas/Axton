#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Swapchain.h"
#include "PipelineAssets.h"
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
			Ref<VertexBuffer> pVertexBuffer;
			Ref<IndexBuffer> pIndexBuffer;
			Ref<Swapchain> pSwapchain;
			Ref<PipelineAssets> Assets;
			std::vector<VertexAttribute> VertexAttributes;

			Specs& setVertPath(std::string path) { VertPath = path; return *this; }
			Specs& setFragPath(std::string path) { FragPath = path; return *this; }
			Specs& setVertexBuffer(Ref<Axton::VertexBuffer> buffer) { pVertexBuffer = buffer; return *this; }
			Specs& setIndexBuffer(Ref<Axton::IndexBuffer> buffer) { pIndexBuffer = buffer; return *this; }
			Specs& setSwapchain(Ref<Swapchain> swapchain) { pSwapchain = swapchain; return *this; }
			Specs& setAssets(Ref<PipelineAssets> assets) { Assets = assets; return *this; }
			Specs& setVertexAttributes(const std::vector<VertexAttribute>& attributes) { VertexAttributes = attributes; return *this; }
			Ref<GraphicsPipeline> Build() { return Create(*this); }
		};

		virtual void Render(uint32_t count) = 0;

		static Ref<GraphicsPipeline> Create(const Specs& specs);
	};
}