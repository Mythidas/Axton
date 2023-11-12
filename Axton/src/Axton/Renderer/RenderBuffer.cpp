#include "axpch.h"
#include "RenderBuffer.h"
#include "RenderEngine.h"
#include "Platform/Vulkan/Client API/VKRenderBuffer.h"

namespace Axton
{
	Ref<RenderBuffer> RenderBuffer::Create(const Specs& specs)
	{
		switch (RenderEngine::GetAPI())
		{
		case RenderEngine::API::None: AX_ASSERT_CORE(false, "API::None currently unsupported!"); return nullptr;
		case RenderEngine::API::Vulkan: return CreateRef<Vulkan::VKRenderBuffer>(specs);
		}

		AX_ASSERT_CORE(false, "Unknown RenderEngine::API! (GraphicsPipeline)");
		return nullptr;
	}
}