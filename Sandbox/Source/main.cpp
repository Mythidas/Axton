#include "Axton.h"
#include "Axton/Core/EntryPoint.h"
#include "RayRenderer.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

using namespace Axton;

class RayTraceLayer : public Layer
{
public:
	virtual void OnAttach() override
	{
		m_ViewportWidth = Application::Get().GetWindow().GetWidth();
		m_ViewportHeight = Application::Get().GetWindow().GetHeight();
	}

	virtual void OnUpdate() override
	{
		Timer timer;

		m_Renderer.TempColor = TempColor;
		m_Renderer.TempLight = TempLight;
		m_Renderer.Resize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render();

		m_LastRenderTime = timer.ElapsedMill();
	}

	virtual void OnRenderUI() override
	{
		ImGui::Begin("Render Stats");
		ImGui::Text("Render Time: %f ms", m_LastRenderTime);
		ImGui::DragFloat3("Sphere Color", glm::value_ptr(TempColor), 0.1f, 0.0f, 1.0f);
		ImGui::DragFloat3("Light Direction", glm::value_ptr(TempLight), 0.2f, -2.0f, 2.0f);
		ImGui::End();

		ImGui::Begin("Camera View");

		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;

		if (m_Renderer.GetFinalImage())
		{
			Ref<Texture2D> image = m_Renderer.GetFinalImage();
			ImGui::Image((ImTextureID)image->GetRendererID(), { (float)image->GetWidth(), (float)image->GetHeight() }, ImVec2(0, 1), ImVec2(1, 0));
		}
		ImGui::End();
	}

private:
	RayRenderer m_Renderer;
	CameraData m_Camera;
	float m_LastRenderTime;

	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	Vector3 TempColor = Vector3(0.0f);
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