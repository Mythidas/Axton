#pragma once

#include "Axton.h"

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

		m_FinalImage = Image2D::Create({ m_ViewportWidth, m_ViewportHeight, 0, ImageFormat::Data::RGBA8, ImageFormat::Access::WRITE_ONLY });
		m_VoxelImage = ComputeShader::Create("C:\\Programming\\Axton\\Axton\\internal\\shaders\\VoxelImageGen.glsl");

		Timer timer("Gen All Chunks");

		//{
		//	VoxModelLoader loader("C:\\Users\\Mythidas\\Desktop\\MagicaVoxel\\MagicaVoxel-0.99.7.1-win64\\vox\\room.vox");
		//	Ref<Chunk> chunk = loader.GenChunk(world, { 0, 0, 50 });
		//	Material mat{ 0.5, 0.1, 1.5, 0.0 };
		//	chunk->MaterialIndex = world.AddMaterial(CreateRef<Material>(mat));
		//}

		{
			VoxModelLoader loader("C:\\Programming\\Axton\\Sandbox\\Assets\\Models\\green_cube.vox");
			Ref<Chunk> chunk = loader.GenChunk(world, { -35, -40, 40 });
			Material mat{ 0.5, 1.0, 0.01, 0.0 };
			chunk->MaterialIndex = world.AddMaterial(CreateRef<Material>(mat));
		}

		{
			VoxModelLoader loader("C:\\Programming\\Axton\\Sandbox\\Assets\\Models\\temple.vox");
			Ref<Chunk> chunk = loader.GenChunk(world, { 0, -10, 50 });
			Material mat{ 0.5, 1.0, 0.01, 0.0 };
			chunk->MaterialIndex = world.AddMaterial(CreateRef<Material>(mat));
		}
	}

	virtual void OnUpdate() override
	{
		m_Camera.OnUpdate();
		m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		world.LoadBuffers(m_Camera);

		m_FinalImage->Resize(m_ViewportWidth, m_ViewportHeight);

		m_VoxelImage->Dispatch(m_ViewportWidth / (uint32_t)8, m_ViewportHeight / (uint32_t)8, 1);
	}

	virtual void OnRenderUI() override
	{
		Vector3 cam = m_Camera.GetPosition();
		ImGui::Begin("Render Stats");
		ImGui::Text("FPS: %f", Time::GetFPS());
		ImGui::Text("Render Time: %f ms", Time::GetMSPerFrame());
		ImGui::End();

		ImGui::Begin("Camera Settings");
		static Vector3 backgroundColor = Vector3(1);
		if (ImGui::ColorEdit3("Background Color", glm::value_ptr(backgroundColor)))
			m_Camera.BackgroundColor(backgroundColor);

		static int renderMode = 0;
		if (ImGui::RadioButton("Render: Full", &renderMode, 0))
			m_Camera.RenderMode(RayCamera::RenderModes::Full);
		if (ImGui::RadioButton("Render: Normals", &renderMode, 1))
			m_Camera.RenderMode(RayCamera::RenderModes::Normals);
		if (ImGui::RadioButton("Render: Albedo", &renderMode, 2))
			m_Camera.RenderMode(RayCamera::RenderModes::Albedo);
		if (ImGui::RadioButton("Render: Difficulty", &renderMode, 3))
			m_Camera.RenderMode(RayCamera::RenderModes::Difficulty);

		static int algorithm = 0;
		if (ImGui::RadioButton("Algo: DDA", &algorithm, 0))
			m_Camera.Algorithm(0);
		if (ImGui::RadioButton("Algo: Oct DFS", &algorithm, 1))
			m_Camera.Algorithm(1);
		ImGui::End();

		ImGui::Begin("Materials");

		for (int i = 0; i < world.Materials().size(); i++)
		{
			ImGui::PushID(i);

			Ref<Material> mat = world.Materials()[i];
			ImGui::DragFloat("Specular", &mat->Specular, 0.1f);
			ImGui::DragFloat("Metallic", &mat->Metallic, 0.1f);
			ImGui::DragFloat("Roughness", &mat->Roughness, 0.1f);
			ImGui::DragFloat("Emissive", &mat->Emissive, 0.1f);

			ImGui::PopID();
		}

		ImGui::End();

		ImGui::Begin("Memory Usage");

		MemTracker instance;
		for (auto const& mem : instance)
		{
			ImGui::Text(mem.first.c_str()); ImGui::SameLine();
			ImGui::Text(": %i", mem.second);
		}

		ImGui::End();

		ImGui::Begin("Camera View");
		m_ViewportWidth = (uint32_t)ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = (uint32_t)ImGui::GetContentRegionAvail().y;

		m_VoxelImage->Barrier();
		if (m_FinalImage)
		{
			ImGui::Image2D((ImTextureID)m_FinalImage->GetRendererID(), { (float)m_FinalImage->GetWidth(), (float)m_FinalImage->GetHeight() }, ImVec2(0, 1.0f), ImVec2(1.0f, 0));
		}
		ImGui::End();
	}

private:
	RayCamera m_Camera;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	Ref<Image2D> m_FinalImage;
	Ref<ComputeShader> m_VoxelImage;
};