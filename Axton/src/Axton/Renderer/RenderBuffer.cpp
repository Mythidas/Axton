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
		case RenderAPI::None: AssertCore(false, "API::None currently unsupported!"); return nullptr;
		case RenderAPI::Vulkan: return CreateRef<Vulkan::VKRenderBuffer>(specs);
		}

		AssertCore(false, "Unknown RenderEngine::API! (RenderBuffer)");
		return nullptr;
	}

	BufferUsage operator|(BufferUsage lhs, BufferUsage rhs)
	{
		return static_cast<BufferUsage>(static_cast<int>(lhs) | static_cast<int>(rhs));
	}

	bool operator&(BufferUsage lhs, BufferUsage rhs)
	{
		return static_cast<bool>(static_cast<int>(lhs) & static_cast<int>(rhs));
	}

	BufferStage operator|(BufferStage lhs, BufferStage rhs)
	{
		return static_cast<BufferStage>(static_cast<int>(lhs) | static_cast<int>(rhs));
	}

	bool operator&(BufferStage lhs, BufferStage rhs)
	{
		return static_cast<bool>(static_cast<int>(lhs) & static_cast<int>(rhs));
	}
}