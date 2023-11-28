#include "axpch.h"
#include "PipelineAssets.h"
#include "RenderEngine.h"
#include "Platform/Vulkan/Client API/VKPipelineAssets.h"

namespace Axton
{
	Ref<PipelineAssets> PipelineAssets::Create(const PipelineAssets::Specs& specs)
	{
		switch (RenderEngine::GetAPI())
		{
		case RenderAPI::None: AssertCore(false, "API::None currently unsupported!"); return nullptr;
		case RenderAPI::Vulkan: return CreateRef<Vulkan::VKPipelineAssets>(specs);
		}

		AssertCore(false, "Unknown RenderEngine::API! (PipelineAssets)");
		return nullptr;
	}
}