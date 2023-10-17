#pragma once

#include "Axton/Renderer/Texture3D.h"

namespace Axton::OpenGL
{
	class OGLTexture3D : public Axton::Texture3D
	{
	public:
		OGLTexture3D(const Texture3DSpecs& specs);
		~OGLTexture3D();

		virtual void Bind(uint32_t slot) const;
		virtual void Unbind() const;

		virtual void SetData(void* data, IVector3 offset);

		virtual int GetWidth() const { return m_Specs.Width; }
		virtual int GetHeight() const { return m_Specs.Height; }
		virtual RendererID GetRendererID() const { return m_RendererID; }

	private:
		RendererID m_RendererID = 0;
		Texture3DSpecs m_Specs;
	};
}