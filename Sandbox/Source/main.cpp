#include <Axton.h>

class ExampleLayer : public Axton::Layer
{
public:
	virtual void OnAttach() override
	{
		Axton::Log::Info("ExampleLayer::OnAttach");
	}
};

class SandboxApplication : public Axton::Application
{
public:
	SandboxApplication()
	{
		PushLayer(new ExampleLayer());
	}

	~SandboxApplication()
	{

	}
};

Axton::Application* Axton::CreateApplication()
{
	return new SandboxApplication();
}