#pragma once

#include "Axton/Core/Defines.h"

namespace Axton
{
	struct VertexAttrib
	{
		enum class VAType
		{
			Float, Float2, Float3, Float4,
			Mat3, Mat4,
			Int, Int2, Int3, Int4,
			Bool
		};

		VAType Type;
		bool Normalized;

		uint32_t GetTypeCount() const;
		uint32_t GetTypeSize() const;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetLayout(const std::vector<VertexAttrib>& attribs) = 0;
		virtual void SetData(const void* data, long long size) = 0;

		static Ref<VertexBuffer> Create(const size_t& size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Ref<IndexBuffer> Create(const uint32_t* indices, const uint32_t count);
	};
}