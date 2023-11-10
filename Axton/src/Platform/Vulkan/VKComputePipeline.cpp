#include "VKComputePipeline.h"

namespace Axton::Vulkan
{
    Ref<VKComputePipeline> VKComputePipeline::Create(Ref<VKGraphicsContext> graphicsContext)
    {
        Ref<VKComputePipeline> computePipeline = CreateRef<VKComputePipeline>();
        computePipeline->m_GraphicsContext = graphicsContext;

        computePipeline->createPipelineLayout();
        computePipeline->createShaderStage();
        computePipeline->createPipeline();

        return computePipeline;
    }

    void VKComputePipeline::createPipelineLayout()
    {

    }

    void VKComputePipeline::createShaderStage()
    {

    }
    
    void VKComputePipeline::createPipeline()
    {

    }
}