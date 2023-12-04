#include "axpch.h"
#include "RenderBuffer.h"
#include "RenderEngine.h"
#include "Platform/Vulkan/VKRenderBuffer.h"

namespace Axton
{
	Ref<RenderBuffer> RenderBuffer::Create(const Specs& specs)
	{
		switch (RenderEngine::GetAPI())
		{
		case RenderAPI::None: AssertCore(false, "API::None currently unsupported!"); return nullptr;
		case RenderAPI::Vulkan: return CreateRef<Vulkan::VKRenderBuffer>(specs);
		}

		AssertCore(false, "Unknown RenderEngine::API! (RenderBuffer)");
		return nullptr;
	}
}