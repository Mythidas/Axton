#include "RayRenderer.h"

RayRenderer::RayRenderer()
{
	m_FinalImage = Image::Create({ 1280, 720, 0, AccessFormat::WRITE_ONLY });
	m_ComputeShader = ComputeShader::Create(
		"C:\\Programming\\Axton\\Axton\\internal\\shaders\\SimpleRayGen.glsl");
}

void RayRenderer::Render(const Camera& camera, const Scene& scene)
{
	m_ActiveScene = &scene;
	m_ActiveCamera = &camera;

	m_ComputeShader->Dispatch((uint32_t)(m_FinalImage->GetWidth() / 8), (uint32_t)(m_FinalImage->GetHeight() / 8), 1);
	m_ComputeShader->Barrier();
}

void RayRenderer::Resize(uint32_t width, uint32_t height)
{
	if (width != m_FinalImage->GetWidth() || height != m_FinalImage->GetHeight())
	{
		m_FinalImage->Resize(width, height);
	} 
}