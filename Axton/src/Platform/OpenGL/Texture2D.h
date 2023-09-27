#pragma once

#include "Axton/Renderer/Texture2D.h"

namespace Axton::OpenGL
{
	class Texture2D : public Axton::Texture2D
	{
	public:
		Texture2D(const Texture2DSpecs& specs);
		~Texture2D();

		virtual void Bind(uint32_t slot) const;
		virtual void Unbind() const;

		virtual void SetData(void* data, uint32_t size);
		virtual void SetData(const std::string& path);

		virtual uint32_t GetWidth() const { return m_Specs.Width; }
		virtual uint32_t GetHeight() const { return m_Specs.Height; }
		virtual RendererID GetRendererID() const { return m_RendererID; }

	private:
		RendererID m_RendererID = 0;
		Texture2DSpecs m_Specs;
	};
}