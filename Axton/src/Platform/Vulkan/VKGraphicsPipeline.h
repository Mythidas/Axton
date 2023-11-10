#pragma once

#include "VKBuffer.h"
#include "Axton/Core/Defines.h"

#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKGraphicsPipeline
	{
	public:
		struct ShaderInfo
		{
			std::string Path;
			vk::ShaderStageFlagBits Stage;
		};

		enum Attribute
		{
			Float2,
			Float3,
			Float4
		};

		struct Specs
		{
			std::vector<ShaderInfo> Shaders;
			std::vector<Attribute> VertexAttributes;

			Specs& setShaders(const std::vector<ShaderInfo>& shaders) { Shaders = shaders; return *this; }
			Specs& setVertexAttributes(const std::vector<Attribute>& attributes) { VertexAttributes = attributes; return *this; }

			Ref<VKGraphicsPipeline> Build() { return VKGraphicsPipeline::Create(*this); }
		};

		static Ref<VKGraphicsPipeline> Create(const Specs& specs);

		void Process();

		vk::Pipeline& GetPipeline() { return m_Pipeline; }
		vk::PipelineLayout& GetPipelineLayout() { return m_Layout; }

	private:
		void createPipelineLayout();
		void createPipeline();

		std::vector<vk::ShaderModule> createTempShaders();
		std::vector<vk::PipelineShaderStageCreateInfo> getShaderStageInfo(const std::vector<vk::ShaderModule>& shaders);

	private:
		Specs m_Specs;

		vk::Pipeline m_Pipeline;
		vk::PipelineLayout m_Layout;
	};
}