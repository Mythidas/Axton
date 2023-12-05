#include "Axton.h"
#include "Axton/Core/EntryPoint.h"

#include "RayTraceLayer.h"
#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

using namespace Axton;

class SimpleLayer : public Axton::Layer
{
	struct Quad
	{
		Vector3 position{ 0.0f };
		Vector3 color{ 1.0f };
	};

	Quad q1;
	Quad q2;

	virtual void OnAttach() override
	{
		
	}

	virtual void OnUpdate() override
	{
		RenderEngine::BeginFrame();
		RenderEngine::DrawQuad(q1.position, Vector4(q1.color, 1.0f));
		RenderEngine::DrawQuad(q2.position, Vector4(q2.color, 1.0f));
		RenderEngine::EndFrame();
	}

	virtual void OnRenderUI() override
	{
		ImGui::Begin("Simple Window");
		ImGui::Text("Yay!");
		ImGui::DragFloat3("Position1", glm::value_ptr(q1.position), 0.1f);
		ImGui::ColorEdit3("Color1", glm::value_ptr(q1.color));
		ImGui::DragFloat3("Position2", glm::value_ptr(q2.position), 0.1f);
		ImGui::ColorEdit3("Color2", glm::value_ptr(q2.color));
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