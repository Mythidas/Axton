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
		case GraphicsAPI::None: AX_ASSERT_CORE(false, "API::None currently unsupported!"); return nullptr;
		case GraphicsAPI::Vulkan: return CreateRef<Vulkan::VKIndexBuffer>(specs);
		}

		AX_ASSERT_CORE(false, "Unknown RenderEngine::API! (IndexBuffer)");
		return nullptr;
    }
}