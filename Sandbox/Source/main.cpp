#include "Axton.h"
#include "Axton/Core/EntryPoint.h"

#include "RayRenderer.h"
#include "Camera.h"
#include "Scene.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

using namespace Axton;

class RayTraceLayer : public Layer
{
public:
	RayTraceLayer()
		: m_Camera(45.0f, 0.01f, 50.0f)
	{}

	virtual void OnAttach() override
	{
		m_ViewportWidth = Application::Get().GetWindow().GetWidth();
		m_ViewportHeight = Application::Get().GetWindow().GetHeight();

		Material material{};
		material.Albedo = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
		material.Roughness = 1.0f;
		m_Scene.AddMaterial(material);

		Material material1{};
		material1.Albedo = Vector4(0.1f, 0.7f, 0.6f, 1.0f);
		material1.Roughness = 1.0f;
		m_Scene.AddMaterial(material1);

		m_Scene.AddSphere({ Vector4(0.0f, -101.0f, 0.0f, 0.0f), 100.0f, 1 });
		m_Scene.AddSphere({ Vector4(0.0f, 1.0f, -2.5f, 0.0f), 0.5f, 0 });
		m_Scene.AddSphere({ Vector4(0.0f, 0.0f, -2.5f, 0.0f), 1.0f, 0 });

		m_Scene.SyncBuffers();
	}

	virtual void OnUpdate() override
	{
		m_LastRenderTime.Reset();

		m_Camera.OnUpdate();
		m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Resize(m_ViewportWidth, m_ViewportHeight);

		m_Renderer.TempLight = TempLight;

		m_Renderer.Render(m_Camera, m_Scene);
	}

	virtual void OnRenderUI() override
	{
		Vector3 cam = m_Camera.GetPosition();
		ImGui::Begin("Render Stats");
		ImGui::DragFloat3("Camera Pos", glm::value_ptr(cam));
		ImGui::Text("FPS: %f", 1.0f / Time::GetDeltaTime());
		ImGui::Text("Render Time: %f ms", m_LastRenderTime.ElapsedMill());
		ImGui::DragFloat3("Light Direction", glm::value_ptr(TempLight), 0.2f, -2.0f, 2.0f);
		ImGui::End();

		ImGui::Begin("Materials");

		for (int i = 0; i < m_Scene.Materials.size(); i++)
		{
			ImGui::PushID(i);

			ImGui::DragFloat3("Albedo", glm::value_ptr(m_Scene.Materials[i].Albedo), 0.05f, 0.0f, 1.0f);
			ImGui::DragFloat("Roughness", &m_Scene.Materials[i].Roughness, 0.05f, 0.0f, 1.0f);

			ImGui::Separator();
			ImGui::PopID();
		}

		ImGui::End();

		ImGui::Begin("Camera View");
		m_ViewportWidth = (uint32_t)ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = (uint32_t)ImGui::GetContentRegionAvail().y;

		if (m_Renderer.GetFinalImage())
		{
			Ref<Image> image = m_Renderer.GetFinalImage();
			ImGui::Image((ImTextureID)image->GetRendererID(), { (float)image->GetWidth(), (float)image->GetHeight() }, ImVec2(0, 1), ImVec2(1, 0));
		}
		ImGui::End();
	}

private:
	RayRenderer m_Renderer;
	Camera m_Camera;
	Scene m_Scene{ 50 };
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	Timer m_LastRenderTime;

	Vector3 TempLight = Vector3(-1.0f);
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