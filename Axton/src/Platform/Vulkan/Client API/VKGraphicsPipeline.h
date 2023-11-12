#pragma once

#include "Axton/Core/Defines.h"
#include "Axton/Renderer/GraphicsPipeline.h"

#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKGraphicsPipeline : public GraphicsPipeline
	{
	public:
		VKGraphicsPipeline(const Specs& specs);

		virtual void Render(uint32_t count) override;

		vk::Pipeline& GetPipeline() { return m_Pipeline; }
		vk::PipelineLayout& GetPipelineLayout() { return m_Layout; }

	private:
		void createPipelineLayout();
		void createPipeline();

		vk::ShaderModule createTempShader(const std::string& path);
		vk::PipelineShaderStageCreateInfo getShaderStageInfo(vk::ShaderModule shader, vk::ShaderStageFlagBits stage);

	private:
		Specs m_Specs;

		vk::Pipeline m_Pipeline;
		vk::PipelineLayout m_Layout;
	};
}