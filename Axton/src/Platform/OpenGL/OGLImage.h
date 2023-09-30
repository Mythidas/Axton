#pragma once

#include "Axton/Renderer/Image.h"

namespace Axton::OpenGL
{
	class OGLImage : public Image
	{
	public:
		OGLImage(const ImageSpecs& specs);
		~OGLImage();

		virtual void Bind() const override;
		virtual void SetData(void* data) const;
		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetWidth() const override { return m_Specs.Width; }
		virtual uint32_t GetHeight() const override { return m_Specs.Height; }
		virtual RendererID GetRendererID() const override { return m_RendererID; }

	private:
		RendererID m_RendererID;
		ImageSpecs m_Specs;
	};
}