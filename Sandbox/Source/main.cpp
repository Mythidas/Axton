#include "Axton.h"
#include "Axton/Core/EntryPoint.h"

#include "Camera.h"
#include "World.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

#include <thread>

using namespace Axton;

struct Material
{
	Vector4 Albedo;
};

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
		Timer timer("Chunk Size Generated: " + std::to_string(size.x * size.y * size.z));

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
						uint32_t rand = Mathf::Random::UInt32(1, 2);
						if (x == size.x - 1)
							newChunk->SetVoxel({ x, y, z }, { (uint8_t)1 });
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
		m_Compute = ComputeShader::Create("C:\\Programming\\Axton\\Axton\\internal\\shaders\\ChunkRaycasterSSBO.glsl");

		{
			AddChunk({ 0, 0, -75 }, { 110, 50, 110 });
		}

		Material white{ {.8, .8, .8, 1} };
		Material black{ {.5, .5, .5, 1} };
		Materials.push_back(white);
		Materials.push_back(black);

		m_MaterialStorage = StorageBuffer::Create({ sizeof(Material) * Materials.size(), BufferUsage::DYNAM_DRAW, 3 });
		m_MaterialStorage->SetData(Materials.data(), sizeof(Material) * Materials.size());

		world.LoadBuffers();
	}

	virtual void OnUpdate() override
	{
		m_Camera.OnUpdate();
		m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);

		m_Image->Resize(m_ViewportWidth * 2, m_ViewportHeight * 2);

		m_Compute->Dispatch(m_ViewportWidth / (uint32_t)8, m_ViewportHeight / (uint32_t)8, 1);
		m_Compute->Barrier();
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
	Ref<ComputeShader> m_Compute;
	Ref<StorageBuffer> m_MaterialStorage;
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