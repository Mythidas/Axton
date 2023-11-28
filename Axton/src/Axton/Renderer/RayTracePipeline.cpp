#include "axpch.h"
#include "RayTracePipeline.h"
#include "RenderEngine.h"
#include "Platform/Vulkan/Client API/VKRayTracePipeline.h"

namespace Axton
{
	Ref<RayTracePipeline> RayTracePipeline::Create(const Specs& specs)
	{
		AssertCore(false, "RayTracePipeline currently unsupported!");

		switch (RenderEngine::GetAPI())
		{
		case RenderAPI::None: AssertCore(false, "API::None currently unsupported!"); return nullptr;
		case RenderAPI::Vulkan: return CreateRef<Vulkan::VKRayTracePipeline>(specs);
		}

		AssertCore(false, "Unknown RenderEngine::API! (GraphicsPipeline)");
		return nullptr;
	}
}