#pragma once

#include "Axton/Renderer/Texture2D.h"

namespace Axton
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const Texture2DSpecs& specs);
		~OpenGLTexture2D();

		virtual void Bind(uint32_t slot) const;
		virtual void Unbind() const;

		virtual void SetData(void* data, uint32_t size);
		virtual void SetData(const std::string& path);

		virtual uint32_t GetWidth() const { return m_Specs.Width; }
		virtual uint32_t GetHeight() const { return m_Specs.Height; }
		virtual RendererID GetRendererID() const { return m_RendererID; }

	private:
		RendererID m_RendererID;
		Texture2DSpecs m_Specs;
	};
}