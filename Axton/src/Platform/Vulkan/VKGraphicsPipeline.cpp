#include "axpch.h"
#include "VKGraphicsPipeline.h"

namespace Axton::Vulkan
{
	Ref<VKGraphicsPipeline> VKGraphicsPipeline::Create(Ref<VKGraphicsContext> graphicsContext, Ref<VKSwapchain> swapchain)
	{
		Ref<VKGraphicsPipeline> graphicsPipeline = CreateRef<VKGraphicsPipeline>();
		graphicsPipeline->m_GraphicsContext = graphicsContext;
		graphicsPipeline->m_Swapchain = swapchain;

		graphicsPipeline->createPipelineLayout();
		graphicsPipeline->createRenderPass();
		graphicsPipeline->createPipeline();

		swapchain->CreateFramebuffers(graphicsPipeline->m_RenderPass);

		graphicsContext->QueueDeletion([graphicsPipeline]()
			{
				graphicsPipeline->m_GraphicsContext->GetDevice().destroy(graphicsPipeline->m_RenderPass);
				graphicsPipeline->m_GraphicsContext->GetDevice().destroy(graphicsPipeline->m_Layout);
				graphicsPipeline->m_GraphicsContext->GetDevice().destroy(graphicsPipeline->m_Pipeline);
			});

		return graphicsPipeline;
	}

	void VKGraphicsPipeline::BeginRender()
	{
		m_GraphicsContext->QueueCommand([this](vk::CommandBuffer& buffer)
			{
				vk::ClearValue clearColor(vk::ClearColorValue({ 0.0f, 0.0f, 0.0f, 1.0f }));
				vk::RenderPassBeginInfo renderPassInfo{};
				renderPassInfo
					.setRenderPass(m_RenderPass)
					.setFramebuffer(m_Swapchain->GetFramebuffer())
					.setRenderArea(vk::Rect2D().setOffset({ 0, 0 }).setExtent(m_Swapchain->GetExtent()))
					.setClearValueCount(1)
					.setPClearValues(&clearColor);

				buffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
				buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_Pipeline);

				vk::Viewport viewport{};
				viewport
					.setX(0.0f)
					.setY(0.0f)
					.setWidth(static_cast<float>(m_Swapchain->GetExtent().width))
					.setHeight(static_cast<float>(m_Swapchain->GetExtent().height))
					.setMinDepth(0.0f)
					.setMaxDepth(1.0f);

				buffer.setViewport(0, { viewport });

				vk::Rect2D scissor{};
				scissor
					.setOffset(vk::Offset2D(0, 0))
					.setExtent(m_Swapchain->GetExtent());

				buffer.setScissor(0, { scissor });
				buffer.draw(3, 1, 0, 0);
			});
	}

	void VKGraphicsPipeline::EndRender()
	{
		m_GraphicsContext->QueueCommand([this](vk::CommandBuffer& buffer)
			{
				buffer.endRenderPass();
			});
	}

	void VKGraphicsPipeline::createPipelineLayout()
	{
		vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo
			.setSetLayoutCount(0)
			.setPushConstantRangeCount(0);

		m_Layout = m_GraphicsContext->GetDevice().createPipelineLayout(pipelineLayoutInfo);
		AX_ASSERT_CORE(m_Layout, "Failed to create PipelineLayout!");
	}

	void VKGraphicsPipeline::createRenderPass()
	{
		vk::AttachmentDescription colorAttachment{};
		colorAttachment
			.setFormat(m_Swapchain->GetFormat())
			.setSamples(vk::SampleCountFlagBits::e1)
			.setLoadOp(vk::AttachmentLoadOp::eClear)
			.setStoreOp(vk::AttachmentStoreOp::eStore)
			.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
			.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
			.setInitialLayout(vk::ImageLayout::eUndefined)
			.setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

		vk::AttachmentReference colorAttachmentRef{};
		colorAttachmentRef
			.setAttachment(0)
			.setLayout(vk::ImageLayout::eColorAttachmentOptimal);

		vk::SubpassDependency dependency{};
		dependency
			.setSrcSubpass(VK_SUBPASS_EXTERNAL)
			.setDstSubpass(0)
			.setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
			.setSrcAccessMask(vk::AccessFlagBits::eNone)
			.setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
			.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite);

		vk::SubpassDescription subpass{};
		subpass
			.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
			.setColorAttachmentCount(1)
			.setPColorAttachments(&colorAttachmentRef);

		vk::RenderPassCreateInfo renderPassInfo{};
		renderPassInfo
			.setAttachmentCount(1)
			.setPAttachments(&colorAttachment)
			.setSubpassCount(1)
			.setPSubpasses(&subpass)
			.setDependencyCount(1)
			.setPDependencies(&dependency);

		m_RenderPass = m_GraphicsContext->GetDevice().createRenderPass(renderPassInfo);
		AX_ASSERT_CORE(m_RenderPass, "Failed to create RenderPass!");
	}

	void VKGraphicsPipeline::createPipeline()
	{
		Ref<VKShader> vertShaderModule = VKShader::Specs().setPath("C:\\Programming\\Axton\\Axton\\internal\\shaders\\vert.spv").setStage(vk::ShaderStageFlagBits::eVertex).Build();
		Ref<VKShader> fragShaderModule = VKShader::Specs().setPath("C:\\Programming\\Axton\\Axton\\internal\\shaders\\frag.spv").setStage(vk::ShaderStageFlagBits::eFragment).Build();

		// Each shader covers a stage in the pipeline

		vk::PipelineShaderStageCreateInfo shaderStages[] = { vertShaderModule->GetStageInfo(), fragShaderModule->GetStageInfo() };

		// Fixed function phases we want control of

		std::vector<vk::DynamicState> dynamicStates =
		{
			vk::DynamicState::eViewport,
			vk::DynamicState::eScissor
		};

		vk::PipelineDynamicStateCreateInfo dynamicState{};
		dynamicState
			.setDynamicStateCount(static_cast<uint32_t>(dynamicStates.size()))
			.setPDynamicStates(dynamicStates.data());

		vk::PipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo
			.setVertexBindingDescriptionCount(0)
			.setPVertexBindingDescriptions(nullptr)
			.setVertexAttributeDescriptionCount(0)
			.setPVertexAttributeDescriptions(nullptr);

		vk::Viewport viewport{};
		viewport
			.setX(0.0f)
			.setY(0.0f)
			.setWidth(static_cast<float>(m_Swapchain->GetExtent().width))
			.setHeight(static_cast<float>(m_Swapchain->GetExtent().height))
			.setMinDepth(0.0f)
			.setMaxDepth(1.0f);

		vk::Rect2D scissor{};
		scissor
			.setOffset(vk::Offset2D(0, 0))
			.setExtent(m_Swapchain->GetExtent());

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
			.setRenderPass(m_RenderPass)
			.setSubpass(0)
			.setBasePipelineHandle(VK_NULL_HANDLE);

		m_Pipeline = m_GraphicsContext->GetDevice().createGraphicsPipeline(VK_NULL_HANDLE, createInfo).value;
		AX_ASSERT_CORE(m_Pipeline, "Failed to create GraphicsPipeline!");

		vertShaderModule->Destroy();
		fragShaderModule->Destroy();
	}
}