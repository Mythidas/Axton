#pragma once

#include "Axton/Renderer/Image1D.h"

namespace Axton::OpenGL
{
	class OGLImage1D : public Image1D
	{
	public:
		OGLImage1D(const Image1DSpecs& specs);
		~OGLImage1D();

		virtual void Bind() const override;
		virtual void SetData(void* data, uint32_t width) override;
		virtual void Resize(uint32_t width) override;

		virtual uint32_t GetWidth() const override { return m_Specs.Width; }
		virtual RendererID GetRendererID() const override { return m_RendererID; }

	private:
		RendererID m_RendererID;
		Image1DSpecs m_Specs;
	};
}