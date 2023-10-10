#pragma once

#include "Axton/Core/Defines.h"

namespace Axton
{
	class UniformBuffer
	{
	public:
		struct Builder
		{
			Builder& Size(uint32_t size) { m_Size = size; return *this; }
			Builder& Binding(uint32_t binding) { m_Binding = binding; return *this; }
			Builder& DebugName(const std::string& name) { m_DebugName = name; return *this; }
			Ref<UniformBuffer> Build() { return UniformBuffer::Create(m_Size, m_Binding, m_DebugName); }

		private:
			uint32_t m_Size{ 0 };
			uint32_t m_Binding{ 0 };
			std::string m_DebugName{ "" };
		};

	public:
		virtual ~UniformBuffer() = default;
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

		static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding = 0, const std::string& debugName = "");
	};
}