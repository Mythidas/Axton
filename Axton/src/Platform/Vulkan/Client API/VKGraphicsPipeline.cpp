#include "axpch.h"
#include "VKGraphicsPipeline.h"
#include "VKRenderBuffer.h"
#include "../VKRenderPass.h"
#include "../VKRenderEngine.h"
#include "../VKSwapchain.h"
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
		if (!specs.Buffers.empty())
		{
			createDescriptorPool();
			createDescriptorSetLayout();
			createDescriptorSets();
		}

		createPipelineLayout();
		createPipeline();

		VKRenderEngine::GetGraphicsContext()->QueueDeletion([this]()
		{
			VKRenderEngine::GetGraphicsContext()->GetDevice().destroy(m_Pipeline);
			VKRenderEngine::GetGraphicsContext()->GetDevice().destroy(m_Layout);
			VKRenderEngine::GetGraphicsContext()->GetDevice().destroy(m_DescriptorPool);
			VKRenderEngine::GetGraphicsContext()->GetDevice().destroy(m_DescriptorSetLayout);
		});
	}

	void VKGraphicsPipeline::Render(uint32_t count)
	{
		vk::CommandBuffer buffer = VKRenderEngine::GetGraphicsContext()->GetCommandBuffer();

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

		if (m_Specs.VertexBuffer)
			m_Specs.VertexBuffer->Bind();

		if (!m_DescriptorSets.empty())
		{
			buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_Layout, 0, { m_DescriptorSets[VKRenderEngine::GetGraphicsContext()->GetCurrentFrame()] }, {  });
		}

		if (m_Specs.IndexBuffer)
		{
			m_Specs.IndexBuffer->Bind();
			buffer.drawIndexed(count, 1, 0, 0, 0);
		}
		else
		{
			buffer.draw(count, 1, 0, 0);
		}
	}

	void VKGraphicsPipeline::createDescriptorPool()
	{
		std::vector<vk::DescriptorPoolSize> poolSizes;

		for (auto& rBuffer : m_Specs.Buffers)
		{
			VKRenderBuffer* vkrBuffer = static_cast<VKRenderBuffer*>(rBuffer.get());
			poolSizes.push_back(vkrBuffer->GetPoolSize());
		}

		vk::DescriptorPoolCreateInfo createInfo{};
		createInfo
			.setPoolSizeCount(static_cast<uint32_t>(poolSizes.size()))
			.setPPoolSizes(poolSizes.data())
			.setMaxSets(static_cast<uint32_t>(VKRenderEngine::MAX_FRAMES_IN_FLIGHT));

		m_DescriptorPool = VKRenderEngine::GetGraphicsContext()->GetDevice().createDescriptorPool(createInfo);
		AX_ASSERT_CORE(m_DescriptorPool, "Failed to create DescriptorPool!");
	}

	void VKGraphicsPipeline::createDescriptorSetLayout()
	{
		std::vector<vk::DescriptorSetLayoutBinding> layoutBindings;

		for (auto& rBuffer : m_Specs.Buffers)
		{
			VKRenderBuffer* vkrBuffer = static_cast<VKRenderBuffer*>(rBuffer.get());
			layoutBindings.push_back(vkrBuffer->GetLayoutBinding());
		}

		vk::DescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo
			.setBindingCount(static_cast<uint32_t>(layoutBindings.size()))
			.setPBindings(layoutBindings.data());

		m_DescriptorSetLayout = VKRenderEngine::GetGraphicsContext()->GetDevice().createDescriptorSetLayout(layoutInfo);
		AX_ASSERT_CORE(m_DescriptorSetLayout, "Failed to create DescriptorSetLayout!");
	}

	void VKGraphicsPipeline::createDescriptorSets()
	{
		std::vector<vk::DescriptorSetLayout> layouts(static_cast<uint32_t>(VKRenderEngine::MAX_FRAMES_IN_FLIGHT), m_DescriptorSetLayout);

		vk::DescriptorSetAllocateInfo allocInfo{};
		allocInfo
			.setDescriptorPool(m_DescriptorPool)
			.setDescriptorSetCount(static_cast<uint32_t>(layouts.size()))
			.setPSetLayouts(layouts.data());

		m_DescriptorSets = VKRenderEngine::GetGraphicsContext()->GetDevice().allocateDescriptorSets(allocInfo);

		for (auto& set : m_DescriptorSets)
		{
			for (auto& buffer : m_Specs.Buffers)
			{
				VKRenderBuffer* rBuffer = static_cast<VKRenderBuffer*>(buffer.get());
				rBuffer->UpdateDescriptorSet(set);
			}
		}
	}

	void VKGraphicsPipeline::createPipelineLayout()
	{
		vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo
			.setSetLayoutCount(m_DescriptorSetLayout ? 1 : 0)
			.setPSetLayouts(&m_DescriptorSetLayout)
			.setPushConstantRangeCount(0);

		m_Layout = VKRenderEngine::GetGraphicsContext()->GetDevice().createPipelineLayout(pipelineLayoutInfo);
		AX_ASSERT_CORE(m_Layout, "Failed to create PipelineLayout!");
	}

	void VKGraphicsPipeline::createPipeline()
	{
		Ref<VKSwapchain> swapchain = VKRenderEngine::GetSwapchain();
		Ref<VKRenderPass> renderPass = VKRenderEngine::GetRenderPass();
		vk::Device device = VKRenderEngine::GetGraphicsContext()->GetDevice();

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
			.setPStages(shaderStages)
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
		device.destroy(vertShader);
		device.destroy(fragShader);
	}
}