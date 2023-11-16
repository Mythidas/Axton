#include "Axton.h"
#include "Axton/Core/EntryPoint.h"

#include "RayTraceLayer.h"
#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

using namespace Axton;

class SimpleLayer : public Axton::Layer
{
	virtual void OnAttach() override
	{
		
	}

	virtual void OnUpdate() override
	{
		
	}

	virtual void OnRenderUI() override
	{
		ImGui::Begin("Simple Window");
		ImGui::Text("Yay!");
		ImGui::End();
	}
};

class SandboxApplication : public Axton::Application
{
public:
	SandboxApplication(const Axton::Window::Specs& specs)
		: Axton::Application(specs)
	{
		PushLayer(new SimpleLayer());
		PushLayer(new RayTraceLayer());
	}

	~SandboxApplication()
	{

	}
};

Axton::Application* Axton::CreateApplication()
{
	Window::Specs window;
	return new SandboxApplication(window.setTitle("Sandbox").setVSync(false));
}