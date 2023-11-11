#include "axpch.h"
#include "VKGraphicsPipeline.h"
#include "VKRenderPass.h"
#include "VKRenderEngine.h"
#include "Axton/Utils/FileSystem.h"

namespace Axton::Vulkan
{
	namespace Utils
	{
		uint32_t getAttributeSize(VKGraphicsPipeline::Attribute attribute)
		{
			switch (attribute)
			{
			case VKGraphicsPipeline::Attribute::Float2: return 4 * 2;
			case VKGraphicsPipeline::Attribute::Float3: return 4 * 3;
			case VKGraphicsPipeline::Attribute::Float4: return 4 * 4;
			}

			return 0;
		}

		vk::Format getFormatInternal(VKGraphicsPipeline::Attribute attribute)
		{
			switch (attribute)
			{
			case VKGraphicsPipeline::Attribute::Float2: return vk::Format::eR32G32Sfloat;
			case VKGraphicsPipeline::Attribute::Float3: return vk::Format::eR32G32B32Sfloat;
			case VKGraphicsPipeline::Attribute::Float4: return vk::Format::eR32G32B32A32Sfloat;
			}

			return vk::Format::eUndefined;
		}

		vk::VertexInputBindingDescription getBindingDescription(const std::vector<VKGraphicsPipeline::Attribute>& attributes)
		{
			uint32_t size = 0;
			for (const auto& attribute : attributes)
			{
				size += Utils::getAttributeSize(attribute);
			}

			vk::VertexInputBindingDescription bindingDescription{};
			bindingDescription
				.setBinding(0)
				.setInputRate(vk::VertexInputRate::eVertex)
				.setStride(size);

			return bindingDescription;
		}

		std::vector<vk::VertexInputAttributeDescription> getAttributeDescriptions(const std::vector<VKGraphicsPipeline::Attribute>& attributes)
		{
			std::vector<vk::VertexInputAttributeDescription> vertexArray(attributes.size());

			uint32_t size = 0;
			for (uint32_t i = 0; i < attributes.size(); i++)
			{
				vk::VertexInputAttributeDescription attributeDescription{};
				attributeDescription
					.setBinding(0)
					.setLocation(i)
					.setFormat(Utils::getFormatInternal(attributes[i]))
					.setOffset(size);

				vertexArray[i] = attributeDescription;
				size += Utils::getAttributeSize(attributes[i]);
			}

			return vertexArray;
		}
	}

	Ref<VKGraphicsPipeline> VKGraphicsPipeline::Create(const Specs& specs)
	{
		Ref<VKGraphicsPipeline> shader = CreateRef<VKGraphicsPipeline>();
		shader->m_Specs = specs;

		shader->createPipelineLayout();
		shader->createPipeline();

		VKRenderEngine::GetGraphicsContext()->QueueDeletion([shader]()
		{
			VKRenderEngine::GetGraphicsContext()->GetDevice().destroy(shader->m_Pipeline);
			VKRenderEngine::GetGraphicsContext()->GetDevice().destroy(shader->m_Layout);
		});

		return shader;
	}

	void VKGraphicsPipeline::Process()
	{
		VKRenderEngine::GetGraphicsContext()->QueueCommand([this](vk::CommandBuffer& buffer)
		{
			Ref<VKSwapchain> swapchain = VKRenderEngine::GetSwapchain();

			buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_Pipeline);

			vk::Viewport viewport{};
			viewport
				.setX(0.0f)
				.setY(0.0f)
				.setWidth(static_cast<float>(swapchain->GetExtent().width))
				.setHeight(static_cast<float>(swapchain->GetExtent().height))
				.setMinDepth(0.0f)
				.setMaxDepth(1.0f);

			buffer.setViewport(0, { viewport });

			vk::Rect2D scissor{};
			scissor
				.setOffset(vk::Offset2D(0, 0))
				.setExtent(swapchain->GetExtent());

			buffer.setScissor(0, { scissor });

			buffer.draw(3, 1, 0, 0);
		});
	}

	void VKGraphicsPipeline::createPipelineLayout()
	{
		vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo
			.setSetLayoutCount(0)
			.setPushConstantRangeCount(0);

		m_Layout = VKRenderEngine::GetGraphicsContext()->GetDevice().createPipelineLayout(pipelineLayoutInfo);
		AX_ASSERT_CORE(m_Layout, "Failed to create PipelineLayout!");
	}

	void VKGraphicsPipeline::createPipeline()
	{
		Ref<VKSwapchain> swapchain = VKRenderEngine::GetSwapchain();
		Ref<VKRenderPass> renderPass = VKRenderEngine::GetRenderPass();
		vk::Device device = VKRenderEngine::GetGraphicsContext()->GetDevice();

		auto shaders = createTempShaders();
		auto shaderStages = getShaderStageInfo(shaders);

		std::vector<vk::DynamicState> dynamicStates =
		{
			vk::DynamicState::eViewport,
			vk::DynamicState::eScissor
		};

		vk::PipelineDynamicStateCreateInfo dynamicState{};
		dynamicState
			.setDynamicStateCount(static_cast<uint32_t>(dynamicStates.size()))
			.setPDynamicStates(dynamicStates.data());

		auto bindingDescription = Utils::getBindingDescription(m_Specs.VertexAttributes);
		auto attributeDescriptions = Utils::getAttributeDescriptions(m_Specs.VertexAttributes);

		vk::PipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo
			.setVertexBindingDescriptionCount(1)
			.setPVertexBindingDescriptions(&bindingDescription)
			.setVertexAttributeDescriptionCount(static_cast<uint32_t>(attributeDescriptions.size()))
			.setPVertexAttributeDescriptions(attributeDescriptions.data());

		vk::Viewport viewport{};
		viewport
			.setX(0.0f)
			.setY(0.0f)
			.setWidth(static_cast<float>(swapchain->GetExtent().width))
			.setHeight(static_cast<float>(swapchain->GetExtent().height))
			.setMinDepth(0.0f)
			.setMaxDepth(1.0f);

		vk::Rect2D scissor{};
		scissor
			.setOffset(vk::Offset2D(0, 0))
			.setExtent(swapchain->GetExtent());

		// Viewport info for Pipeline

		vk::PipelineViewportStateCreateInfo viewportState{};
		viewportState
			.setViewportCount(1)
			.setScissorCount(1)
			.setPViewports(&viewport)
			.setPScissors(&scissor);

		// Input method for geometry

		vk::PipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly
			.setTopology(vk::PrimitiveTopology::eTriangleList)
			.setPrimitiveRestartEnable(VK_FALSE);

		// Rasterization phase

		vk::PipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer
			.setDepthClampEnable(VK_FALSE)
			.setRasterizerDiscardEnable(VK_FALSE)
			.setPolygonMode(vk::PolygonMode::eFill)
			.setLineWidth(1.0f)
			.setCullMode(vk::CullModeFlagBits::eBack)
			.setFrontFace(vk::FrontFace::eClockwise)
			.setDepthBiasEnable(VK_FALSE)
			.setDepthBiasConstantFactor(0.0f)
			.setDepthBiasClamp(0.0f)
			.setDepthBiasSlopeFactor(0.0f);

		// Multisampling phase

		vk::PipelineMultisampleStateCreateInfo multisampling{};
		multisampling
			.setSampleShadingEnable(VK_FALSE)
			.setRasterizationSamples(vk::SampleCountFlagBits::e1)
			.setMinSampleShading(1.0f)
			.setPSampleMask(nullptr)
			.setAlphaToCoverageEnable(VK_FALSE)
			.setAlphaToOneEnable(VK_FALSE);

		// Color blending phase

		vk::PipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment
			.setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA)
			.setBlendEnable(VK_FALSE)
			.setSrcColorBlendFactor(vk::BlendFactor::eOne)
			.setDstColorBlendFactor(vk::BlendFactor::eZero)
			.setColorBlendOp(vk::BlendOp::eAdd)
			.setSrcAlphaBlendFactor(vk::BlendFactor::eOne)
			.setDstAlphaBlendFactor(vk::BlendFactor::eZero)
			.setAlphaBlendOp(vk::BlendOp::eAdd);

		vk::PipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending
			.setLogicOpEnable(VK_FALSE)
			.setLogicOp(vk::LogicOp::eCopy)
			.setAttachmentCount(1)
			.setPAttachments(&colorBlendAttachment)
			.setBlendConstants({ 0.0f, 0.0f, 0.0f, 0.0f });

		// Connecting all the pipeline phases

		vk::GraphicsPipelineCreateInfo createInfo{};
		createInfo
			.setStageCount(2)
			.setPStages(shaderStages.data())
			.setPViewportState(&viewportState)
			.setPVertexInputState(&vertexInputInfo)
			.setPInputAssemblyState(&inputAssembly)
			.setPRasterizationState(&rasterizer)
			.setPMultisampleState(&multisampling)
			.setPColorBlendState(&colorBlending)
			.setPDynamicState(&dynamicState)
			.setLayout(m_Layout)
			.setRenderPass(renderPass->GetRenderPass())
			.setSubpass(0)
			.setBasePipelineHandle(VK_NULL_HANDLE);

		m_Pipeline = device.createGraphicsPipeline(VK_NULL_HANDLE, createInfo).value;
		AX_ASSERT_CORE(m_Pipeline, "Failed to create GraphicsPipeline!");

		// Destroy temp shaders
		for (auto& shader : shaders)
		{
			device.destroy(shader);
		}
	}

	std::vector<vk::ShaderModule> VKGraphicsPipeline::createTempShaders()
	{
		std::vector<vk::ShaderModule> shaders(m_Specs.Shaders.size());

		for (size_t i = 0; i < shaders.size(); i++)
		{
			vk::Device device = VKRenderEngine::GetGraphicsContext()->GetDevice();

			FileSystem fs(m_Specs.Shaders[i].Path);
			std::vector<char> buffer = fs.ToSignedBuffer();

			vk::ShaderModuleCreateInfo createInfo{};
			createInfo
				.setCodeSize(buffer.size())
				.setPCode(reinterpret_cast<const uint32_t*>(buffer.data()));

			shaders[i] = device.createShaderModule(createInfo);
		}

		return shaders;
	}

	std::vector<vk::PipelineShaderStageCreateInfo> VKGraphicsPipeline::getShaderStageInfo(const std::vector<vk::ShaderModule>& shaders)
	{
		std::vector<vk::PipelineShaderStageCreateInfo> stages(shaders.size());

		for (size_t i = 0; i < stages.size(); i++)
		{
			stages[i] = vk::PipelineShaderStageCreateInfo();
			stages[i]
				.setStage(m_Specs.Shaders[i].Stage)
				.setModule(shaders[i])
				.setPName("main");
		}

		return stages;
	}
}