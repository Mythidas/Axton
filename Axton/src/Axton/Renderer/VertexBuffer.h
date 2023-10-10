#pragma once

#include "Axton/Core/Defines.h"

namespace Axton
{
	class VertexBuffer
	{
	public:
		struct Attribute
		{
			enum class AttributeFormat
			{
				Float, Float2, Float3, Float4,
				Mat3, Mat4,
				Int, Int2, Int3, Int4,
				Bool
			};

			AttributeFormat Format;
			bool Normalized;

			uint32_t GetTypeCount() const;
			uint32_t GetTypeSize() const;
		};

	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetLayout(const std::vector<Attribute>& attribs) = 0;
		virtual void SetData(const void* data, long long size) = 0;

		static Ref<VertexBuffer> Create(const size_t& size);
	};
}