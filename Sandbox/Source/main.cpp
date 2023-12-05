#include "Axton.h"
#include "Axton/Core/EntryPoint.h"

#include "RayTraceLayer.h"
#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

using namespace Axton;

class SimpleLayer : public Axton::Layer
{
	Vector3 position{ 0.0f };
	Vector3 color{ 1.0f };

	virtual void OnAttach() override
	{
		
	}

	virtual void OnUpdate() override
	{
		RenderEngine::BeginFrame();
		RenderEngine::DrawQuad(position, Vector4(color, 1.0f));
		RenderEngine::EndFrame();
	}

	virtual void OnRenderUI() override
	{
		ImGui::Begin("Simple Window");
		ImGui::Text("Yay!");
		ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f);
		ImGui::ColorEdit3("Color", glm::value_ptr(color));
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
		//PushLayer(new RayTraceLayer());
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