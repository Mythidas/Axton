#pragma once

#include "Axton/Renderer/RendererAPI.h"
#include "Axton/Renderer/StorageBuffer.h"

namespace Axton::OpenGL
{
	class OGLStorageBuffer : public StorageBuffer
	{
	public:
		OGLStorageBuffer(const StorageBuffer::Specs& specs);
		~OGLStorageBuffer();

		virtual void* MapBufferPtr() const override;
		virtual void UnmapBufferPtr() const override;
		virtual void SetData(const void* data, size_t size, uint32_t offset = 0) override;

	public:
		RendererID m_RendererID;
		std::string m_DebugName;
	};
}