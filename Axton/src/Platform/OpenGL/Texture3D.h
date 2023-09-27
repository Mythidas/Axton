#pragma once

#include "Axton/Renderer/Texture3D.h"

namespace Axton::OpenGL
{
	class Texture3D : public Axton::Texture3D
	{
	public:
		Texture3D(const Texture3DSpecs& specs);
		~Texture3D();

		virtual void Bind(uint32_t slot) const;
		virtual void Unbind() const;

		virtual void SetData(void* data, int size);

		virtual int GetWidth() const { return m_Specs.Width; }
		virtual int GetHeight() const { return m_Specs.Height; }
		virtual RendererID GetRendererID() const { return m_RendererID; }

	private:
		RendererID m_RendererID = 0;
		Texture3DSpecs m_Specs;
	};
}