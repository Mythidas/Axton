#include "axpch.h"
#include "ComputePipeline.h"
#include "RenderEngine.h"
#include "Platform/Vulkan/Client API/VKComputePipeline.h"

namespace Axton
{
	Ref<ComputePipeline> ComputePipeline::Create(const Specs& specs)
	{
		switch (RenderEngine::GetAPI())
		{
		case GraphicsAPI::None: AX_ASSERT_CORE(false, "API::None currently unsupported!"); return nullptr;
		case GraphicsAPI::Vulkan: return CreateRef<Vulkan::VKComputePipeline>(specs);
		}

		AX_ASSERT_CORE(false, "Unknown RenderEngine::API! (GraphicsPipeline)");
		return nullptr;
	}
}