#include "axpch.h"
#include "VertexBuffer.h"
#include "RenderEngine.h"
#include "Platform/Vulkan/Client API/VKVertexBuffer.h"

namespace Axton
{
	Ref<VertexBuffer> VertexBuffer::Create(const VertexBuffer::Specs& specs)
	{
		switch (RenderEngine::GetAPI())
		{
		case RenderEngine::API::None: AX_ASSERT_CORE(false, "API::None currently unsupported!"); return nullptr;
		case RenderEngine::API::Vulkan: return CreateRef<Vulkan::VKVertexBuffer>(specs);
		}

		AX_ASSERT_CORE(false, "Unknown RenderEngine::API! (VertexBuffer)");
		return nullptr;
	}
}