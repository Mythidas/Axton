#pragma once

#include "Axton/Renderer/Image2D.h"

namespace Axton::OpenGL
{
	class OGLImage2D : public Image2D
	{
	public:
		OGLImage2D(const Image2DSpecs& specs);
		~OGLImage2D();

		virtual void Bind() const override;
		virtual void SetData(void* data, uint32_t width, uint32_t height) override;
		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetWidth() const override { return m_Specs.Width; }
		virtual uint32_t GetHeight() const override { return m_Specs.Height; }
		virtual RendererID GetRendererID() const override { return m_RendererID; }

	private:
		RendererID m_RendererID;
		Image2DSpecs m_Specs;
	};
}