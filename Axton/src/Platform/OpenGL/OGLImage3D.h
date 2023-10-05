#pragma once

#include "Axton/Renderer/Image3D.h"

namespace Axton::OpenGL
{
	class OGLImage3D : public Image3D
	{
	public:
		OGLImage3D(const Image3DSpecs& specs);
		~OGLImage3D();

		virtual void Bind() const override;
		virtual void SetData(void* data) override;
		virtual void SetData(void* data, uint32_t width, uint32_t height, uint32_t depth) override;
		virtual void Resize(uint32_t width, uint32_t height, uint32_t depth) override;

		virtual uint32_t GetWidth() const override { return m_Specs.Width; }
		virtual uint32_t GetHeight() const override { return m_Specs.Height; }
		virtual RendererID GetRendererID() const override { return m_RendererID; }

	private:
		RendererID m_RendererID;
		Image3DSpecs m_Specs;
	};
}