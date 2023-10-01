#pragma once

#include "Axton/Renderer/RendererAPI.h"
#include "Axton/Renderer/StorageBuffer.h"

namespace Axton::OpenGL
{
	class OGLStorageBuffer : public StorageBuffer
	{
	public:
		OGLStorageBuffer(uint32_t size, BufferUsage usage, uint32_t binding);
		~OGLStorageBuffer();

		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

	public:
		RendererID m_RendererID;
	};
}