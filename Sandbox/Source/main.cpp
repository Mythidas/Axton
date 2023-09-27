#include "Axton.h"
#include "Axton/Core/EntryPoint.h"

#include "RayRenderer.h"

using namespace Axton;

class RayTraceLayer : public Layer
{
public:
	virtual void OnAttach() override
	{
		m_Renderer.Resize(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
	}

	virtual void OnUpdate() override
	{
		m_Renderer.Resize(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
		m_Renderer.Render();

		Renderer2D::BeginFrame(m_Camera);

		if (m_Renderer.GetFinalImage())
		{
			Renderer2D::DrawQuadUI(Vector2(0.0f), Vector2(2.0f), Vector4(1.0f), m_Renderer.GetFinalImage());
		}

		Renderer2D::EndFrame();
	}

private:
	RayRenderer m_Renderer;
	CameraData m_Camera;
};

class SandboxApplication : public Axton::Application
{
public:
	SandboxApplication()
	{
		PushLayer(new RayTraceLayer());
	}

	~SandboxApplication()
	{

	}
};

Axton::Application* Axton::CreateApplication()
{
	return new SandboxApplication();
}