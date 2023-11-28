#include "axpch.h"
#include "VKGraphicsPipeline.h"
#include "VKRenderBuffer.h"
#include "VKPipelineAssets.h"
#include "VKSwapchain.h"
#include "VKRenderPass.h"
#include "../VKRenderEngine.h"
#include "../VKUtils.h"
#include "Axton/Utils/FileSystem.h"

namespace Axton::Vulkan
{
	namespace Utils
	{
		uint32_t getAttributeSize(VertexAttribute attribute)
		{
			switch (attribute)
			{
			case VertexAttribute::Float2: return 4 * 2;
			case VertexAttribute::Float3: return 4 * 3;
			case VertexAttribute::Float4: return 4 * 4;
			}

			return 0;
		}

		vk::Format getFormatInternal(VertexAttribute attribute)
		{
			switch (attribute)
			{
			case VertexAttribute::Float2: return vk::Format::eR32G32Sfloat;
			case VertexAttribute::Float3: return vk::Format::eR32G32B32Sfloat;
			case VertexAttribute::Float4: return vk::Format::eR32G32B32A32Sfloat;
			}

			return vk::Format::eUndefined;
		}

		vk::VertexInputBindingDescription getBindingDescription(const std::vector<VertexAttribute>& attributes)
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

		std::vector<vk::VertexInputAttributeDescription> getAttributeDescriptions(const std::vector<VertexAttribute>& attributes)
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

	VKGraphicsPipeline::VKGraphicsPipeline(const Specs& specs)
		: m_Specs(specs)
	{
		createPipelineLayout();
		createPipeline();
	}

	VKGraphicsPipeline::~VKGraphicsPipeline()
	{
		VKRenderEngine::GetDevice().waitIdle();
		VKRenderEngine::GetDevice().destroy(m_Pipeline);
		VKRenderEngine::GetDevice().destroy(m_Layout);
	}

	void VKGraphicsPipeline::Render(uint32_t count)
	{
		VKRenderEngine::GetGraphicsContext()->QueueGraphicsCommand([count, this](vk::CommandBuffer& buffer)
		{
			VKSwapchain& vkSwapchain = static_cast<VKSwapchain&>(*m_Specs.pSwapchain.get());

			buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_Pipeline);

			vk::Viewport viewport{};
			viewport
				.setX(0.0f)
				.setY(0.0f)
				.setWidth(static_cast<float>(vkSwapchain.GetExtent().Width))
				.setHeight(static_cast<float>(vkSwapchain.GetExtent().Height))
				.setMinDepth(0.0f)
				.setMaxDepth(1.0f);

			buffer.setViewport(0, { viewport });

			vk::Rect2D scissor{};
			scissor
				.setOffset(vk::Offset2D(0, 0))
				.setExtent(vk::Extent2D(vkSwapchain.GetExtent().Width, vkSwapchain.GetExtent().Height));

			buffer.setScissor(0, { scissor });

			if (m_Specs.pVertexBuffer)
				m_Specs.pVertexBuffer->Bind();

			if (!m_Specs.Assets->Empty())
			{
				VKPipelineAssets* assets = static_cast<VKPipelineAssets*>(m_Specs.Assets.get());
				buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_Layout, 0, { assets->GetSet() }, {});
			}

			if (m_Specs.pIndexBuffer)
			{
				m_Specs.pIndexBuffer->Bind();
				buffer.drawIndexed(count, 1, 0, 0, 0);
			}
			else
			{
				buffer.draw(count, 1, 0, 0);
			}
		});
	}

	void VKGraphicsPipeline::createPipelineLayout()
	{
		vk::DescriptorSetLayout layout = static_cast<VKPipelineAssets*>(m_Specs.Assets.get())->GetLayout();

		vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo
			.setSetLayoutCount(layout ? 1 : 0)
			.setPSetLayouts(&layout)
			.setPushConstantRangeCount(0);

		m_Layout = VKRenderEngine::GetDevice().createPipelineLayout(pipelineLayoutInfo);
		AssertCore(m_Layout, "Failed to create PipelineLayout!");
	}

	void VKGraphicsPipeline::createPipeline()
	{
		VKSwapchain& vkSwapchain = static_cast<VKSwapchain&>(*m_Specs.pSwapchain.get());
		VKRenderPass& vkRenderPass = static_cast<VKRenderPass&>(*m_Specs.pSwapchain->GetRenderPass().get());
		vk::Device device = VKRenderEngine::GetDevice();

		vk::ShaderModule vertShader = VKUtils::CreateShader(m_Specs.VertPath);
		vk::ShaderModule fragShader = VKUtils::CreateShader(m_Specs.FragPath);

		vk::PipelineShaderStageCreateInfo shaderStages[] =
		{
			VKUtils::CreateShaderStageInfo(vertShader, vk::ShaderStageFlagBits::eVertex),
			VKUtils::CreateShaderStageInfo(fragShader, vk::ShaderStageFlagBits::eFragment)
		};

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
			.setWidth(static_cast<float>(vkSwapchain.GetExtent().Width))
			.setHeight(static_cast<float>(vkSwapchain.GetExtent().Height))
			.setMinDepth(0.0f)
			.setMaxDepth(1.0f);

		vk::Rect2D scissor{};
		scissor
			.setOffset(vk::Offset2D(0, 0))
			.setExtent(vk::Extent2D(vkSwapchain.GetExtent().Width, vkSwapchain.GetExtent().Height));

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
			.setPStages(shaderStages)
			.setPViewportState(&viewportState)
			.setPVertexInputState(&vertexInputInfo)
			.setPInputAssemblyState(&inputAssembly)
			.setPRasterizationState(&rasterizer)
			.setPMultisampleState(&multisampling)
			.setPColorBlendState(&colorBlending)
			.setPDynamicState(&dynamicState)
			.setLayout(m_Layout)
			.setRenderPass(vkRenderPass)
			.setSubpass(0)
			.setBasePipelineHandle(VK_NULL_HANDLE);

		m_Pipeline = device.createGraphicsPipeline(VK_NULL_HANDLE, createInfo).value;
		AssertCore(m_Pipeline, "Failed to create GraphicsPipeline!");

		// Destroy temp shaders
		device.destroy(vertShader);
		device.destroy(fragShader);
	}
}