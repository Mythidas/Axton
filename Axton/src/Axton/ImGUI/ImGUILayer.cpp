#include "axpch.h"
#include "ImGUILayer.h"
#include "Axton/Renderer/RendererAPI.h"
#include "Platform/Vulkan/VKImGUILayer.h"

namespace Axton
{
	ImGUILayer* ImGUILayer::Create()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::Vulkan:  return new Vulkan::VKImGUILayer();
		}

		AX_ASSERT_CORE(false, "Unknown RendererAPI!");
		return nullptr;
	}
}