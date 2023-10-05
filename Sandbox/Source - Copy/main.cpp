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

	Ref<Image3D> image3D;

	virtual void OnAttach() override
	{
		m_ViewportWidth = Application::Get().GetWindow().GetWidth();
		m_ViewportHeight = Application::Get().GetWindow().GetHeight();

		Material material{};
		material.Albedo = Vector4(0.7, 0.5, 1, 1);
		material.Roughness = 1.0f;
		m_Scene.AddMaterial(material);

		Material material1{};
		material1.Albedo = Vector4(0.1f, 0.7f, 0.6f, 1.0f);
		material1.Roughness = 1.0f;
		m_Scene.AddMaterial(material1);

		m_Scene.AddBox({ Vector3(0), Vector3(2), 0 });
		//m_Scene.AddBox({ Vector3(0, -10.5, 0), Vector3(10), 1 });

		image3D = Image3D::Create({ 4, 4, 4, 1, AccessFormat::READ_ONLY, ImageFormat::R8 });
		uint8_t value[4][4][4] = 
		{
			{ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
			{ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
			{ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
			{ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 1, 0, 0, 0 } }
		};
		image3D->SetData(&value);

		m_Scene.SyncBuffers();
	}

	virtual void OnUpdate() override
	{
		m_LastRenderTime.Reset();

		m_Camera.OnUpdate();
		m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Resize(m_ViewportWidth, m_ViewportHeight);

		m_Renderer.Render(m_Camera, m_Scene);
	}

	virtual void OnRenderUI() override
	{
		Vector3 cam = m_Camera.GetPosition();
		ImGui::Begin("Render Stats");
		if (ImGui::Button("Update Scene"))
			m_Scene.SyncBuffers();
		ImGui::Text("FPS: %f", 1.0f / Time::GetDeltaTime());
		ImGui::Text("Render Time: %f ms", m_LastRenderTime.ElapsedMill());
		ImGui::DragFloat3("Camera Pos", glm::value_ptr(cam));
		ImGui::End();

		ImGui::Begin("Materials");

		for (int i = 0; i < m_Scene.Materials.size(); i++)
		{
			ImGui::PushID(i);

			if (ImGui::DragFloat3("Albedo", glm::value_ptr(m_Scene.Materials[i].Albedo), 0.05f, 0.0f, 1.0f))
				m_Scene.SyncBuffers();
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