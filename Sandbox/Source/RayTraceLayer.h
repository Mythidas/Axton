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

		m_FinalImage = Image::Specs()
			.setBinding(0)
			.setExtents({ m_ViewportWidth, m_ViewportHeight, 1 })
			.setFormat(ImageFormat::RGBA8)
			.setStages(ImageStages::Compute)
			.setUsage(ImageUsage::ShaderWrite | ImageUsage::ShaderRead)
			.setType(ImageType::e2D)
			.Build();

		m_CompAssets = PipelineAssets::Specs()
			.setBuffers({ world.m_ChunkStorage, world.m_VoxelStorage, world.m_MaterialStorage, m_Camera.m_CameraBuffer })
			.setImages({ m_FinalImage })
			.Build();

		m_CompPipeline = ComputePipeline::Specs()
			.setShaderPath("C:\\Programming\\Axton\\Axton\\internal\\shaders\\VoxelImageGen.spv")
			.setAssets(m_CompAssets)
			.Build();

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

		if (m_FinalImage->Resize({ m_ViewportWidth, m_ViewportHeight, 1 }))
		{
			m_CompAssets->Rebuild();
			return;
		}
			

		m_CompPipeline->Dispatch(m_ViewportWidth / (uint32_t)8, m_ViewportHeight / (uint32_t)8, 1);
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
		ImGui::Text("Render Mode");
		if (ImGui::RadioButton("Full", &renderMode, 0))
			m_Camera.RenderMode(RayCamera::RenderModes::Full);
		if (ImGui::RadioButton("Normals", &renderMode, 1))
			m_Camera.RenderMode(RayCamera::RenderModes::Normals);
		if (ImGui::RadioButton("Albedo", &renderMode, 2))
			m_Camera.RenderMode(RayCamera::RenderModes::Albedo);
		if (ImGui::RadioButton("Difficulty", &renderMode, 3))
			m_Camera.RenderMode(RayCamera::RenderModes::Difficulty);

		ImGui::Text("Sample Settings");
		static int samples = 1;
		if (ImGui::SliderInt("Samples", &samples, 1, 30))
		{
			uint32_t settings = m_Camera.SampleSettings();
			settings = Bit::U32_4x8(uint8_t(samples), Bit::Get<uint8_t>(settings, 8), 0, 0);
			m_Camera.SampleSettings(settings);
		}
		static int bounces = 2;
		if (ImGui::SliderInt("Bounces", &bounces, 2, 30))
		{
			uint32_t settings = m_Camera.SampleSettings();
			settings = Bit::U32_4x8(Bit::Get<uint8_t>(settings, 0), bounces, 0, 0);
			m_Camera.SampleSettings(settings);
		}

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

		if (m_FinalImage)
		{
			ImGui::Image((ImTextureID)m_FinalImage->GetRendererID(), { (float)m_FinalImage->GetExtents().x, (float)m_FinalImage->GetExtents().y }, ImVec2(0, 1.0f), ImVec2(1.0f, 0));
		}
		ImGui::End();
	}

private:
	RayCamera m_Camera;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	Ref<Image> m_FinalImage;
	Ref<PipelineAssets> m_CompAssets;
	Ref<ComputePipeline> m_CompPipeline;
};