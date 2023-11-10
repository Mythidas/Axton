#include "axpch.h"
#include "VKVertexArray.h"

namespace Axton::Vulkan
{
	namespace Utils
	{
		uint32_t GetFormatSize(VKVertexArray::Format format)
		{
			switch (format)
			{
			case VKVertexArray::Float2: return 4 * 2;
			case VKVertexArray::Float3: return 4 * 3;
			case VKVertexArray::Float4: return 4 * 4;
			}

			return 0;
		}

		vk::Format GetFormatInternal(VKVertexArray::Format format)
		{
			switch (format)
			{
			case VKVertexArray::Float2: return vk::Format::eR32G32Sfloat;
			case VKVertexArray::Float3: return vk::Format::eR32G32B32Sfloat;
			case VKVertexArray::Float4: return vk::Format::eR32G32B32A32Sfloat;
			}

			return vk::Format::eUndefined;
		}
	}

	Ref<VKVertexArray> VKVertexArray::Create(const std::vector<Format>& attributes)
	{
		Ref<VKVertexArray> vertexArray = CreateRef<VKVertexArray>();

		uint32_t size = 0;
		for (const auto& format : attributes)
		{
			vk::VertexInputAttributeDescription attributeDescription{};
			attributeDescription
				.setBinding(0)
				.setLocation(static_cast<uint32_t>(vertexArray->m_AttributeDescriptions.size()))
				.setFormat(Utils::GetFormatInternal(format))
				.setOffset(size);

			vertexArray->m_AttributeDescriptions.push_back(attributeDescription);

			size += Utils::GetFormatSize(format);
		}

		vertexArray->m_BindingDescription
			.setBinding(0)
			.setInputRate(vk::VertexInputRate::eVertex)
			.setStride(size);

		return vertexArray;
	}
}