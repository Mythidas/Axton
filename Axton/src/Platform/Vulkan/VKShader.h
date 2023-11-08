#pragma once

#include "Axton/Core/Defines.h"

#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKShader
	{
	public:
		struct Specs
		{
			std::string Path;
			vk::ShaderStageFlagBits Stage;

			Specs& setPath(const std::string& path) { Path = path; return *this; }
			Specs& setStage(vk::ShaderStageFlagBits stage) { Stage = stage; return *this; }

			Ref<VKShader> Build() { return VKShader::Create(*this); }
		};

		static Ref<VKShader> Create(const Specs& specs);

		void Destroy();

		vk::PipelineShaderStageCreateInfo GetStageInfo();
		vk::ShaderModule& GetNative() { return m_Shader; }

	private:
		vk::ShaderModule m_Shader;
		Specs m_Specs;
	};
}