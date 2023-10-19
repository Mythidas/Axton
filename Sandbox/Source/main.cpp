#include "Axton.h"
#include "Axton/Core/EntryPoint.h"

#include "RayCamera.h"
#include "World.h"
#include "VoxModelLoader.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

using namespace Axton;

class RayTraceLayer : public Layer
{
public:
	RayTraceLayer()
		: m_Camera({ 45.0f, 0.01f, 50.0f })
	{}

	std::vector<Material> Materials;
	World world{ 10000000 };

	virtual void OnAttach() override
	{
		m_ViewportWidth = Application::Get().GetWindow().GetWidth();
		m_ViewportHeight = Application::Get().GetWindow().GetHeight();

		m_Image = Image2D::Create({ m_ViewportWidth, m_ViewportHeight, 0, AccessFormat::READ_WRITE, ImageFormat::RGBA8 });
		m_VoxelImage = ComputeShader::Create("C:\\Programming\\Axton\\Axton\\internal\\shaders\\VoxelImageGen_SVO.glsl");

		Timer timer("Gen All Chunks");

		{
			VoxModelLoader loader("C:\\Users\\Mythidas\\Downloads\\MagicaVoxel-0.99.6.4-win64\\MagicaVoxel-0.99.6.4-win64\\vox\\doom.vox");
			loader.GenChunk(world, { -300, 0, 50 });
		}

		{
			VoxModelLoader loader("C:\\Users\\Mythidas\\Downloads\\MagicaVoxel-0.99.6.4-win64\\MagicaVoxel-0.99.6.4-win64\\vox\\castle.vox");
			loader.GenChunk(world, { -200, 0, 50 });
		}

		{
			VoxModelLoader loader("C:\\Users\\Mythidas\\Downloads\\MagicaVoxel-0.99.6.4-win64\\MagicaVoxel-0.99.6.4-win64\\vox\\chr_knight.vox");
			loader.GenChunk(world, { -100, 0, 50 });
		}

		{
			VoxModelLoader loader("C:\\Users\\Mythidas\\Downloads\\MagicaVoxel-0.99.6.4-win64\\MagicaVoxel-0.99.6.4-win64\\vox\\monu1.vox");
			loader.GenChunk(world, { 0, 0, 50 });
		}

		{
			VoxModelLoader loader("C:\\Users\\Mythidas\\Downloads\\MagicaVoxel-0.99.6.4-win64\\MagicaVoxel-0.99.6.4-win64\\vox\\monu2.vox");
			loader.GenChunk(world, { 100, 0, 50 });
		}

		{
			VoxModelLoader loader("C:\\Users\\Mythidas\\Downloads\\MagicaVoxel-0.99.6.4-win64\\MagicaVoxel-0.99.6.4-win64\\vox\\room.vox");
			loader.GenChunk(world, { 250, 0, 50 });
		}

		{
			VoxModelLoader loader("C:\\Users\\Mythidas\\Downloads\\MagicaVoxel-0.99.6.4-win64\\MagicaVoxel-0.99.6.4-win64\\vox\\teapot.vox");
			loader.GenChunk(world, { 400, 0, 50 });
		}

		{
			VoxModelLoader loader("C:\\Users\\Mythidas\\Downloads\\MagicaVoxel-0.99.6.4-win64\\MagicaVoxel-0.99.6.4-win64\\vox\\monu2.vox");
			loader.GenChunk(world, { 500, 0, 50 });
		}

		{
			VoxModelLoader loader("C:\\Users\\Mythidas\\Downloads\\MagicaVoxel-0.99.6.4-win64\\MagicaVoxel-0.99.6.4-win64\\vox\\room.vox");
			loader.GenChunk(world, { 650, 0, 50 });
		}

		{
			VoxModelLoader loader("C:\\Users\\Mythidas\\Downloads\\MagicaVoxel-0.99.6.4-win64\\MagicaVoxel-0.99.6.4-win64\\vox\\teapot.vox");
			loader.GenChunk(world, { 800, 0, 50 });
		}
	}

	virtual void OnUpdate() override
	{
		m_Camera.OnUpdate();
		m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		world.LoadBuffers(m_Camera);

		m_Image->Resize(m_ViewportWidth, m_ViewportHeight);

		m_VoxelImage->Dispatch(m_ViewportWidth / (uint32_t)8, m_ViewportHeight / (uint32_t)8, 1);
	}

	virtual void OnRenderUI() override
	{
		Vector3 cam = m_Camera.GetPosition();
		ImGui::Begin("Render Stats");
		ImGui::Text("FPS: %f", Time::GetFPS());
		ImGui::Text("Render Time: %f ms", Time::GetMSPerFrame());
		static int renderMode = 0;
		if (ImGui::RadioButton("Render: Full", &renderMode, 0)) m_Camera.SetRenderPass(RayCamera::RenderPass::Full);
		if (ImGui::RadioButton("Render: Normals", &renderMode, 1)) m_Camera.SetRenderPass(RayCamera::RenderPass::Normals);
		if (ImGui::RadioButton("Render: Albedo", &renderMode, 2)) m_Camera.SetRenderPass(RayCamera::RenderPass::Albedo);
		ImGui::End();

		ImGui::Begin("Buffer Stats");

		MemTracker instance;
		for (auto& slot : instance)
		{
			ImGui::Text("Slot: %s", slot.first.c_str()); ImGui::SameLine();
			ImGui::Text("| Size: %i mb", slot.second / 1000000);
		}

		ImGui::End();

		ImGui::Begin("Camera View");
		m_ViewportWidth = (uint32_t)ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = (uint32_t)ImGui::GetContentRegionAvail().y;

		m_VoxelImage->Barrier();
		if (m_Image)
		{
			ImGui::Image2D((ImTextureID)m_Image->GetRendererID(), { (float)m_Image->GetWidth(), (float)m_Image->GetHeight() }, ImVec2(0, 1.0f), ImVec2(1.0f, 0));
		}
		ImGui::End();
	}

private:
	RayCamera m_Camera;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	Ref<Image2D> m_Image;
	Ref<ComputeShader> m_VoxelImage;
};

class SandboxApplication : public Axton::Application
{
public:
	SandboxApplication(const Window::Builder& window)
		: Axton::Application(window)
	{
		PushLayer(new RayTraceLayer());
	}

	~SandboxApplication()
	{

	}
};

Axton::Application* Axton::CreateApplication()
{
	Window::Builder window;
	window.Title("Sandbox").VSync(false);

	return new SandboxApplication(window);
}