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
		case GraphicsAPI::None: AX_ASSERT_CORE(false, "API::None currently unsupported!"); return nullptr;
		case GraphicsAPI::Vulkan: return CreateRef<Vulkan::VKPipelineAssets>(specs);
		}

		AX_ASSERT_CORE(false, "Unknown RenderEngine::API! (PipelineAssets)");
		return nullptr;
	}
}