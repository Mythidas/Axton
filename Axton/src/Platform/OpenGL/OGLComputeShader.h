#pragma once

#include "Axton/Renderer/ComputeShader.h"

namespace Axton::OpenGL
{
	class OGLComputeShader : public ComputeShader
	{
	public:
		OGLComputeShader(const std::string& srcPath);
		~OGLComputeShader();

		virtual void Dispatch(uint32_t width, uint32_t height, uint32_t depth) const override;
		virtual void Barrier() const override;

	private:
		RendererID m_RendererID;
	};
}