#include "axpch.h"
#include "IndexBuffer.h"
#include "RenderEngine.h"
#include "Platform/Vulkan/Client API/VKIndexBuffer.h"

namespace Axton
{
    Ref<IndexBuffer> IndexBuffer::Create(const IndexBuffer::Specs& specs)
    {
		switch (RenderEngine::GetAPI())
		{
		case RenderAPI::None: AssertCore(false, "API::None currently unsupported!"); return nullptr;
		case RenderAPI::Vulkan: return CreateRef<Vulkan::VKIndexBuffer>(specs);
		}

		AssertCore(false, "Unknown RenderEngine::API! (IndexBuffer)");
		return nullptr;
    }
}