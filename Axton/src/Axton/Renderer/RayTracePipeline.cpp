#include "axpch.h"
#include "RayTracePipeline.h"
#include "RenderEngine.h"
#include "Platform/Vulkan/Client API/VKRayTracePipeline.h"

namespace Axton
{
	Ref<RayTracePipeline> RayTracePipeline::Create(const Specs& specs)
	{
		switch (RenderEngine::GetAPI())
		{
		case GraphicsAPI::None: AX_ASSERT_CORE(false, "API::None currently unsupported!"); return nullptr;
		case GraphicsAPI::Vulkan: return CreateRef<Vulkan::VKRayTracePipeline>(specs);
		}

		AX_ASSERT_CORE(false, "Unknown RenderEngine::API! (GraphicsPipeline)");
		return nullptr;
	}
}