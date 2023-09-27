#pragma once

#include "Axton/Renderer/RendererAPI.h"
#include "Axton/Renderer/Framebuffer.h"

namespace Axton::OpenGL
{
	class Framebuffer : public Axton::Framebuffer
	{
	public:
		Framebuffer(const FramebufferSpec& specs);
		virtual ~Framebuffer();

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void Resize(uint32_t width, uint32_t height);
		virtual void Invalidate() override;

		virtual const RendererID GetColorAttachmentID(uint32_t index = 0) const override;
		virtual const FramebufferSpec& GetSpec() const override { return m_Spec; }
		
	private:
		RendererID m_RendererID = 0;
		FramebufferSpec m_Spec;

		std::vector<RendererID> m_ColorAttachments;
		RendererID m_DepthAttachment = 0;

		std::vector<FramebufferTextureSpec> m_ColorAttachmentSpecs;
		FramebufferTextureSpec m_DepthAttachmentSpec;
	};
}