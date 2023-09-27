#pragma once

#include "Axton/Core/Defines.h"
#include "RendererAPI.h"

namespace Axton
{
	enum class FramebufferTextureFormat
	{
		None = 0,
		RGBA8,
		DEPTH24STENCIL8,
		DEPTH = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpec
	{
		FramebufferTextureSpec() = default;
		FramebufferTextureSpec(FramebufferTextureFormat format)
			: TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
	};

	struct FramebufferAttachmentSpec
	{
		FramebufferAttachmentSpec() = default;
		FramebufferAttachmentSpec(std::initializer_list<FramebufferTextureSpec> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpec> Attachments;
	};

	struct FramebufferSpec
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		uint32_t Samples = 1;
		FramebufferAttachmentSpec Attachments;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void Invalidate() = 0;

		virtual const RendererID GetColorAttachmentID(uint32_t index) const = 0;
		virtual const FramebufferSpec& GetSpec() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpec& spec);
	};
}