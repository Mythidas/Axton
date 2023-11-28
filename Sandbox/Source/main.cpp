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
	SandboxApplication(const Axton::Application::Specs& specs)
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
	Window::Specs window{};
	window
		.setTitle("Sandbox")
		.setVSync(false);

	RenderEngine::Specs render{};
	render
		.setAPI(Axton::RenderAPI::Vulkan)
		.setExtensions(Axton::RenderExtension::Compute | Axton::RenderExtension::Graphics);

	Axton::Application::Specs appSpecs{};
	appSpecs
		.setWindowSpecs(window)
		.setRenderSpecs(render);

	return new SandboxApplication(appSpecs);
}