#pragma once

#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKVertexArray
	{
	public:
		enum Format
		{
			Float2,
			Float3,
			Float4
		};

	public:
		static Ref<VKVertexArray> Create(const std::vector<Format>& attributes);

		vk::VertexInputBindingDescription& GetBindingDescription() { return m_BindingDescription; }
		const std::vector<vk::VertexInputAttributeDescription>& GetAttributeDescriptions() { return m_AttributeDescriptions; }

	private:
		vk::VertexInputBindingDescription m_BindingDescription;
		std::vector<vk::VertexInputAttributeDescription> m_AttributeDescriptions;
	};
}