#include "axpch.h"
#include "RenderEngine.h"
#include "Platform/Vulkan/VKRenderEngine.h"

namespace Axton
{
	RenderAPI RenderEngine::s_API = RenderAPI::Vulkan;
	Ref<Swapchain> RenderEngine::s_Swapchain = nullptr;
	Ref<RenderPass> RenderEngine::s_RenderPass = nullptr;

	namespace Render
	{
		struct Data
		{

		};
	}

	static Render::Data s_Data;

	void RenderEngine::Construct()
	{
		s_RenderPass = RenderPass::Specs().Build();
		s_Swapchain = Swapchain::Specs()
			.setRenderPass(s_RenderPass)
			.Build();
	}

	void RenderEngine::Destruct()
	{
		s_RenderPass = nullptr;
		s_Swapchain = nullptr;
	}

	void RenderEngine::BeginBatch()
	{

	}

	void RenderEngine::EndBatch()
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

	RenderExtension operator|(RenderExtension lhs, RenderExtension rhs)
	{
		return static_cast<RenderExtension>(int(lhs) | int(rhs));
	}

	bool operator&(RenderExtension lhs, RenderExtension rhs)
	{
		return static_cast<bool>(int(lhs) & int(rhs));
	}
}