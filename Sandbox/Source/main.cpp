#include "Axton.h"
#include "Axton/Core/EntryPoint.h"

//#include "RayTraceLayer.h"
#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

class SimpleLayer : public Axton::Layer
{
	struct Vertex
	{
		glm::vec2 pos;
		glm::vec3 col;
		glm::vec2 tex;
	};

	Axton::Ref<Axton::GraphicsPipeline> pipeline;
	Axton::Ref<Axton::VertexBuffer> vertBuffer;
	Axton::Ref<Axton::IndexBuffer> indexBuffer;
	Axton::Ref<Axton::RenderBuffer> uniformBuffer;
	glm::vec3 color;
	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices = {
		0, 1, 2, 2, 3, 0
	};

	virtual void OnAttach() override
	{
		Axton::Log::Info("Simple Layer");

		vertices =
		{
			{ glm::vec2(-0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
			{ glm::vec2( 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
			{ glm::vec2( 0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
			{ glm::vec2(-0.5f,  0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f) }
		};

		vertBuffer = Axton::VertexBuffer::Specs()
			.setSize(sizeof(Vertex) * vertices.size())
			.Build();

		vertBuffer->SetData(vertices.data(), sizeof(Vertex) * vertices.size(), 0);

		indexBuffer = Axton::IndexBuffer::Specs()
			.setSize(sizeof(uint16_t) * indices.size())
			.Build();

		indexBuffer->SetData(indices.data(), sizeof(uint16_t) * indices.size(), 0);

		uniformBuffer = Axton::RenderBuffer::Specs()
			.setBinding(0)
			.setSize(sizeof(glm::vec3))
			.setStorage(Axton::BufferStorage::Host)
			.setRate(Axton::BufferRate::PerFrame)
			.setStages(Axton::BufferStage::Vertex)
			.setUsage(Axton::BufferUsage::Uniform)
			.Build();

		color = { 1.0f, 1.0f, 1.0f };
		uniformBuffer->SetData(&color, sizeof(color), 0);
		
		pipeline = Axton::GraphicsPipeline::Specs()
			.setVertPath("C:\\Programming\\Axton\\Axton\\internal\\shaders\\vert.spv")
			.setFragPath("C:\\Programming\\Axton\\Axton\\internal\\shaders\\frag.spv")
			.setVertexBuffer(vertBuffer)
			.setIndexBuffer(indexBuffer)
			.setBuffers({ uniformBuffer })
			.setVertexAttributes({
				Axton::VertexAttribute::Float2,
				Axton::VertexAttribute::Float3,
				Axton::VertexAttribute::Float2
			})
			.Build();
	}

	virtual void OnUpdate() override
	{
		pipeline->Render(static_cast<uint32_t>(indices.size()));

		if (Axton::Input::IsKeyPressed(Axton::KeyCode::W))
		{
			vertices[0].pos.x += 0.3f * Axton::Time::GetDeltaTime();
			vertBuffer->SetData(vertices.data(), sizeof(Vertex) * vertices.size(), 0);
		}
	}

	virtual void OnRenderUI() override
	{
		ImGui::Begin("Simple Window");
		if (ImGui::ColorEdit3("Simple Text", glm::value_ptr(color)))
			uniformBuffer->SetData(&color, sizeof(color), 0);
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