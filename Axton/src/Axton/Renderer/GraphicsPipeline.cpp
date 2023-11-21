#include "axpch.h"
#include "GraphicsPipeline.h"
#include "RenderEngine.h"
#include "Platform/Vulkan/Client API/VKGraphicsPipeline.h"

namespace Axton
{
	Ref<GraphicsPipeline> GraphicsPipeline::Create(const Specs& specs)
	{
		switch (RenderEngine::GetAPI())
		{
		case RenderAPI::None: AX_ASSERT_CORE(false, "API::None currently unsupported!"); return nullptr;
		case RenderAPI::Vulkan: return CreateRef<Vulkan::VKGraphicsPipeline>(specs);
		}

		AX_ASSERT_CORE(false, "Unknown RenderEngine::API! (GraphicsPipeline)");
		return nullptr;
	}
}