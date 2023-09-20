#include <Axton.h>
#include <Axton/Core/EntryPoint.h>
#include "ExampleLayer.h"

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