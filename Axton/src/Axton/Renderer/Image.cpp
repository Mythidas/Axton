#include "axpch.h"
#include "Image.h"
#include "RenderEngine.h"
#include "Platform/Vulkan/Client API/VKImage.h"

namespace Axton
{
	Ref<Image> Image::Create(const Specs& specs)
	{
		switch (RenderEngine::GetAPI())
		{
		case GraphicsAPI::None: AX_ASSERT_CORE(false, "API::None currently unsupported!"); return nullptr;
		case GraphicsAPI::Vulkan: return CreateRef<Vulkan::VKImage>(specs);
		}

		AX_ASSERT_CORE(false, "Unknown RenderEngine::API! (Image)");
		return nullptr;
	}

	ImageUsage operator|(ImageUsage lhs, ImageUsage rhs)
	{
		return static_cast<ImageUsage>(static_cast<int>(lhs) | static_cast<int>(rhs));
	}

	bool operator&(ImageUsage lhs, ImageUsage rhs)
	{
		return static_cast<bool>(static_cast<int>(lhs) & static_cast<int>(rhs));
	}

	ImageStages operator|(ImageStages lhs, ImageStages rhs)
	{
		return static_cast<ImageStages>(static_cast<int>(lhs) | static_cast<int>(rhs));
	}

	bool operator&(ImageStages lhs, ImageStages rhs)
	{
		return static_cast<bool>(static_cast<int>(lhs) & static_cast<int>(rhs));
	}
}