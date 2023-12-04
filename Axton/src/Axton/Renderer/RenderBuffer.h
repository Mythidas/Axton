#pragma once

#include "Axton/Core/Defines.h"
#include "Axton/Utils/Flags.h"

namespace Axton
{
	enum class BufferUsage
	{
		Vertex			= 1 << 0,
		Index			= 1 << 1,
		Uniform			= 1 << 2,
		ShaderStorage	= 1 << 3
	};

	AX_FLAG_OPERATORS(BufferUsage)

	enum class BufferStage
	{
		Vertex		= 1 << 0,
		Fragment	= 1 << 1,
		Compute		= 1 << 2
	};

	AX_FLAG_OPERATORS(BufferStage)

	enum class BufferStorage
	{
		Host,
		Local
	};

	enum class BufferRate
	{
		Static,
		Dyanamic,
		PerFrame
	};

	class RenderBuffer
	{
	public:
		struct Specs
		{
			size_t Size{ 0 };
			uint32_t Binding{ 0 };
			BufferUsage Usage{};
			BufferStage Stages{};
			BufferStorage Storage{ BufferStorage::Host };
			BufferRate Rate{ BufferRate::Static };

			Specs& setSize(size_t size) { Size = size; return *this; }
			Specs& setBinding(uint32_t binding) { Binding = binding; return *this; }
			Specs& setUsage(BufferUsage usage) { Usage = usage; return *this; }
			Specs& setStages(BufferStage stage) { Stages = stage; return *this; }
			Specs& setStorage(BufferStorage storage) { Storage = storage; return *this; }
			Specs& setRate(BufferRate rate) { Rate = rate; return *this; }
			Ref<RenderBuffer> Build() { return Create(*this); }
		};

		virtual void SetData(void* data, size_t size, uint32_t offset) = 0;
		virtual void* MapBufferPtr(uint32_t offset, size_t size) = 0;
		virtual void UnmapBufferPtr() = 0;

		virtual void Bind() const = 0;

		virtual const Specs& GetSpecs() const = 0;

		static Ref<RenderBuffer> Create(const Specs& specs);
	};
}