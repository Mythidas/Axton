#pragma once

#include "Axton/Renderer/UniformBuffer.h"
#include "Axton/Renderer/RendererAPI.h"

namespace Axton::OpenGL
{
	class UniformBuffer : public Axton::UniformBuffer
	{
	public:
		UniformBuffer(uint32_t size, uint32_t binding);
		~UniformBuffer();

		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

	private:
		RendererID m_RendererID = 0;
	};
}