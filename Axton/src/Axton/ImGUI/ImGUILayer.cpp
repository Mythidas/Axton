#include "axpch.h"
#include "ImGUILayer.h"
#include "Axton/Renderer/RenderEngine.h"
#include "Platform/Vulkan/VKImGUILayer.h"

namespace Axton
{
	ImGUILayer* ImGUILayer::Create()
	{
		switch (RenderEngine::GetAPI())
		{
		case RenderAPI::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::Vulkan:  return new Vulkan::VKImGUILayer();
		}

		AX_ASSERT_CORE(false, "Unknown RendererAPI!");
		return nullptr;
	}
}