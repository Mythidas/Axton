#include "Axton.h"
#include "Axton/Core/EntryPoint.h"

#include "RayCamera.h"
#include "World.h"
#include "VoxModelLoader.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

#include <thread>

using namespace Axton;

void parallel_function(int start, int end, Ref<Chunk> layer)
{
	for (int i = start; i < end; i++)
	{
		uint32_t rand = Mathf::Random::UInt32(1, 100);
		layer->SetVoxel(i, { (uint8_t)(rand > 2 ? 0 : rand) });
	}
	return;
}

class RayTraceLayer : public Layer
{
public:
	RayTraceLayer()
		: m_Camera({ 45.0f, 0.01f, 50.0f })
	{}

	std::vector<Material> Materials;
	World world{ 10000000 };

	void AddChunk(Vector3 position, Vector3 size)
	{
		Ref<Chunk> newChunk = world.CreateChunk(position, UVector3(size.x, size.y, size.z));

		bool mt = false;

		world.BeginEdit(newChunk);
		if (!mt)
		{
			for (int z = 0; z < size.z; z++)
			{
				for (int y = 0; y < size.y; y++)
				{
					for (int x = 0; x < size.x; x++)
					{
						uint32_t rand = Mathf::Random::UInt32(0, 1);
						if (y < 24)
							newChunk->SetVoxel({ x, y, z }, { rand });
						else
							newChunk->SetVoxel({ x, y, z }, { 255 });
					}
				}
			}
		}
		else
		{
			const int num_threads = 8;
			const int array_size = size.x * size.y * size.z;

			std::vector<std::thread> threads;
			int chunk_size = array_size / num_threads;

			for (int i = 0; i < num_threads; i++)
			{
				int start = i * chunk_size;
				int end = (i + 1) * chunk_size;
				threads.emplace_back([start, end, newChunk]()
					{
						parallel_function(start, end, newChunk);
					});
			}

			for (auto& thread : threads)
				thread.join();
		}
		world.EndEdit(newChunk);
	}

	virtual void OnAttach() override
	{
		m_ViewportWidth = Application::Get().GetWindow().GetWidth();
		m_ViewportHeight = Application::Get().GetWindow().GetHeight();

		m_Image = Image2D::Create({ m_ViewportWidth * 2, m_ViewportHeight * 2, 0, AccessFormat::READ_WRITE, ImageFormat::RGBA8 });
		m_VoxelImage = ComputeShader::Create("C:\\Programming\\Axton\\Axton\\internal\\shaders\\VoxelImageGen.glsl");

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

		world.LoadBuffers();
	}

	virtual void OnUpdate() override
	{
		m_Camera.OnUpdate();
		m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);

		m_Image->Resize(m_ViewportWidth * 2, m_ViewportHeight * 2);

		m_VoxelImage->Dispatch(m_ViewportWidth / (uint32_t)8, m_ViewportHeight / (uint32_t)8, 1);
		m_VoxelImage->Barrier();
	}

	virtual void OnRenderUI() override
	{
		Vector3 cam = m_Camera.GetPosition();
		ImGui::Begin("Render Stats");
		ImGui::Text("FPS: %f", 1.0f / Time::GetDeltaTime());
		ImGui::Text("Render Time: %f ms", Time::GetDeltaTime());
		static int renderMode = 0;
		ImGui::RadioButton("Render: Full", &renderMode, 0);
		ImGui::RadioButton("Render: Normals", &renderMode, 1);
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

		if (m_Image)
		{
			switch (renderMode)
			{
			case 0: ImGui::Image2D((ImTextureID)m_Image->GetRendererID(), { (float)m_Image->GetWidth() / 2, (float)m_Image->GetHeight() / 2 }, ImVec2(0, 0.5f), ImVec2(0.5f, 0)); break;
			case 1: ImGui::Image2D((ImTextureID)m_Image->GetRendererID(), { (float)m_Image->GetWidth() / 2, (float)m_Image->GetHeight() / 2 }, ImVec2(0.5f, 1), ImVec2(1, 0.5f)); break;
			}
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