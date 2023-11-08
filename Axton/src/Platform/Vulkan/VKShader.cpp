#include "axpch.h"
#include "VKShader.h"
#include "VKRendererAPI.h"
#include "Axton/Utils/FileSystem.h"

namespace Axton::Vulkan
{
	Ref<VKShader> VKShader::Create(const Specs& specs)
	{
		Ref<VKShader> shader = CreateRef<VKShader>();
		shader->m_Specs = specs;

		FileSystem fs(specs.Path);
		std::vector<char> buffer = fs.ToSignedBuffer();

		vk::ShaderModuleCreateInfo createInfo{};
		createInfo
			.setCodeSize(buffer.size())
			.setPCode(reinterpret_cast<const uint32_t*>(buffer.data()));

		shader->m_Shader = VKRendererAPI::GetGraphicsContext().GetDevice().createShaderModule(createInfo);

		return shader;
	}

	void VKShader::Destroy()
	{
		VKRendererAPI::GetGraphicsContext().GetDevice().destroy(m_Shader);
	}

	vk::PipelineShaderStageCreateInfo VKShader::GetStageInfo()
	{
		vk::PipelineShaderStageCreateInfo createInfo{};
		createInfo
			.setStage(m_Specs.Stage)
			.setModule(m_Shader)
			.setPName("main");

		return createInfo;
	}
}