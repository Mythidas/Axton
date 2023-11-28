#include "axpch.h"
#include "Texture.h"
#include "RenderEngine.h"
#include "Platform/Vulkan/Client API/VKTexture.h"

namespace Axton
{
	Ref<Texture> Texture::Create(const Specs& specs)
	{
		switch (RenderEngine::GetAPI())
		{
		case RenderAPI::None: AssertCore(false, "API::None currently unsupported!"); return nullptr;
		case RenderAPI::Vulkan: return CreateRef<Vulkan::VKTexture>(specs);
		}

		AssertCore(false, "Unknown RenderEngine::API! (Texture)");
		return nullptr;
	}

	TextureUsage operator|(TextureUsage lhs, TextureUsage rhs)
	{
		return static_cast<TextureUsage>(static_cast<int>(lhs) | static_cast<int>(rhs));
	}

	bool operator&(TextureUsage lhs, TextureUsage rhs)
	{
		return static_cast<bool>(static_cast<int>(lhs) & static_cast<int>(rhs));
	}

	TextureStages operator|(TextureStages lhs, TextureStages rhs)
	{
		return static_cast<TextureStages>(static_cast<int>(lhs) | static_cast<int>(rhs));
	}

	bool operator&(TextureStages lhs, TextureStages rhs)
	{
		return static_cast<bool>(static_cast<int>(lhs) & static_cast<int>(rhs));
	}
}