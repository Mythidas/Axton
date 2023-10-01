#pragma once

#include "Axton/Core/Defines.h"

namespace Axton
{
	enum class BufferUsage
	{
		DYNAM_DRAW,
		DYNAM_READ,
		DYNAM_COPY,
		STATIC_DRAW,
		STATIC_READ,
		STATIC_COPY,
	};

	class StorageBuffer
	{
	public:
		virtual ~StorageBuffer() = default;
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;
		
		static Ref<StorageBuffer> Create(uint32_t size, BufferUsage usage, uint32_t binding = 0);
	};
}