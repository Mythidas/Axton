#include "Axton.h"
#include "Axton/Core/EntryPoint.h"

//#include "RayTraceLayer.h"
#include "imgui/imgui.h"

class SimpleLayer : public Axton::Layer
{
	struct Vertex
	{
		glm::vec2 pos;
		glm::vec3 col;
		glm::vec2 tex;
	};

	Axton::Ref<Axton::GraphicsPipeline> pipeline;
	Axton::Ref<Axton::RenderBuffer> vertBuffer;
	std::vector<Vertex> vertices;

	virtual void OnAttach() override
	{
		Axton::Log::Info("Simple Layer");

		vertices =
		{
			{ glm::vec2(0.0f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
			{ glm::vec2(0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
			{ glm::vec2(-0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) }
		};

		vertBuffer = Axton::RenderBuffer::Specs().setSize(sizeof(Vertex) * vertices.size()).setUsage(Axton::BufferUsage::Vertex).Build();
		vertBuffer->SetData(vertices.data(), sizeof(Vertex) * vertices.size());
		
		pipeline = Axton::GraphicsPipeline::Specs()
			.setVertPath("C:\\Programming\\Axton\\Axton\\internal\\shaders\\vert.spv")
			.setFragPath("C:\\Programming\\Axton\\Axton\\internal\\shaders\\frag.spv")
			.setBuffers({ vertBuffer })
			.setVertexAttributes({
				Axton::VertexAttribute::Float2,
				Axton::VertexAttribute::Float3,
				Axton::VertexAttribute::Float2
			})
			.Build();
	}

	virtual void OnUpdate() override
	{
		pipeline->Render(3);
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