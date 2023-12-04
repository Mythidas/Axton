#include "axpch.h"
#include "RenderEngine.h"
#include "GraphicsPipeline.h"
#include "Platform/Vulkan/VKRenderEngine.h"

namespace Axton
{
	RenderAPI RenderEngine::s_API = RenderAPI::Vulkan;
	Ref<Swapchain> RenderEngine::s_Swapchain = nullptr;
	Ref<RenderPass> RenderEngine::s_RenderPass = nullptr;

	namespace Render
	{
		static const size_t MaxQuadCount = 1000;
		static const size_t MaxVertexCount = MaxQuadCount * 4;
		static const size_t MaxIndexCount = MaxQuadCount * 6;
		static const size_t MaxTextures = 32;

		struct Vertex
		{

		};

		struct Data
		{
			Ref<GraphicsPipeline> QuadPipeline;
		};
	}

	static Render::Data s_Data;

	void RenderEngine::Construct()
	{
		s_RenderPass = RenderPass::Specs().Build();
		s_Swapchain = Swapchain::Specs()
			.setRenderPass(s_RenderPass)
			.Build();

		Ref<RenderBuffer> vertexBuffer = RenderBuffer::Specs()
			.setBinding(0)
			.setRate(BufferRate::PerFrame)
			.setSize(sizeof(Render::Vertex) * Render::MaxVertexCount)
			.setStages(BufferStage::Vertex)
			.setStorage(BufferStorage::Local)
			.setUsage(BufferUsage::Vertex)
			.Build();

		Ref<RenderBuffer> indexBuffer = RenderBuffer::Specs()
			.setBinding(0)
			.setRate(BufferRate::Static)
			.setSize(sizeof(uint16_t) * Render::MaxIndexCount)
			.setStages(BufferStage::Vertex)
			.setStorage(BufferStorage::Local)
			.setUsage(BufferUsage::Index)
			.Build();

		uint16_t* indices = new uint16_t[Render::MaxIndexCount];
		uint16_t offset = 0;
		for (size_t i = 0; i < Render::MaxIndexCount; i += 6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		indexBuffer->SetData(indices, indexBuffer->GetSpecs().Size, 0);
		delete[] indices;

		Ref<PipelineAssets> quadPiplineAssets = PipelineAssets::Specs()
			.setBuffers({})
			.Build();

		s_Data.QuadPipeline = GraphicsPipeline::Specs()
			.setVertPath()
			.setFragPath()
			.setSwapchain(s_Swapchain)
			.setIndexBuffer(indexBuffer)
			.setVertexBuffer(vertexBuffer)
			.setAssets(quadPiplineAssets)
			.setVertexAttributes({
				
			})
			.Build();
	}

	void RenderEngine::Destruct()
	{
		s_RenderPass = nullptr;
		s_Swapchain = nullptr;
	}

	void RenderEngine::BeginFrame(const Camera& camera)
	{

	}

	void RenderEngine::EndFrame()
	{
		
	}

	void RenderEngine::DrawQuad(const Vector3& position, const Vector4& color)
	{

	}

	Scope<RenderEngine> RenderEngine::Create(void* windowHandle, const Specs& specs)
	{
		s_API = specs.API;
		Scope<RenderEngine> renderEngine = nullptr;


		switch (s_API)
		{
		case RenderAPI::None:    AssertCore(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::Vulkan:  renderEngine = CreateScope<Vulkan::VKRenderEngine>(windowHandle, specs); break;
		}

		Construct();
		AssertCore(renderEngine != nullptr, "Uknown Renderer::API!");
		return renderEngine;
	}
}