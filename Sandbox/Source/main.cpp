#include "Axton.h"
#include "Axton/Core/EntryPoint.h"

//#include "RayTraceLayer.h"
#include "imgui/imgui.h"

class SimpleLayer : public Axton::Layer
{
	virtual void OnAttach() override
	{
		Axton::Log::Info("Simple Layer");

		struct Vertex
		{
			glm::vec2 pos;
			glm::vec3 col;
			glm::vec2 tex;
		};

		std::vector<Vertex> vertices =
		{
			{ glm::vec2(0.0f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
			{ glm::vec2(0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
			{ glm::vec2(-0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) }
		};
	}

	virtual void OnRenderUI() override
	{
		ImGui::Begin("Simple Window");
		ImGui::Text("Simple Text");
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
		//PushLayer(new RayTraceLayer());
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