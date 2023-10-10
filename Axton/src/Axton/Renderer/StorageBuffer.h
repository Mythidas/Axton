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
		struct Specs
		{
			size_t Size{ 0 };
			BufferUsage Usage{ BufferUsage::DYNAM_DRAW };
			uint32_t Binding{ 0 };
			std::string DebugName{ "" };
		};

		struct Builder
		{
			Builder& Size(size_t size) { m_Specs.Size = size; return *this; }
			Builder& Usage(BufferUsage usage) { m_Specs.Usage = usage; return *this; }
			Builder& Binding(uint32_t binding) { m_Specs.Binding = binding; return *this; }
			Builder& DebugName(const std::string& name) { m_Specs.DebugName = name; return *this; }
			Ref<StorageBuffer> Build() { return StorageBuffer::Create(m_Specs); }
		private:
			Specs m_Specs;
		};

	public:
		virtual ~StorageBuffer() = default;

		virtual void* MapBufferPtr() const = 0;
		virtual void UnmapBufferPtr() const = 0;
		virtual void SetData(const void* data, size_t size, uint32_t offset = 0) = 0;
		
		static Ref<StorageBuffer> Create(const Specs& specs);
	};
}