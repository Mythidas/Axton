#include "Axton.h"
#include "Axton/Core/EntryPoint.h"

#include "Camera.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

#include <thread>
#include <iostream>
#include <future>
#include <ppl.h>

using namespace Axton;

struct Voxel
{
	uint8_t MaterialIndex;
};

struct Chunk
{
	struct Buffer
	{
		alignas(16) Vector3 MinExtent;
		alignas(16) Vector3 MaxExtent;
		alignas(16) IVector3 VoxelOffset;
		uint32_t VoxelIndex;
		uint32_t MaterialLookup[64];
	};

	Vector3 Position;
	UVector3 Extents;
	Ref<Image3D> VoxelData;

	Chunk(Vector3 position, UVector3 extents)
		: Position(position), Extents(extents)
	{
		VoxelData = Image3D::Create({ extents.x, extents.y, extents.z, 1, AccessFormat::READ_ONLY, ImageFormat::R8 });
		std::vector<uint8_t> blank;
		blank.resize(extents.x * extents.y * extents.z, (uint8_t)1);
		VoxelData->SetData(blank.data());
	}

	Chunk::Buffer GetBuffer(uint32_t index) const
	{
		Chunk::Buffer buffer{};
		buffer.MinExtent = Position - Vector3(Extents) * 0.5f * 1.0f;
		buffer.MaxExtent = Position + Vector3(Extents) * 0.5f * 1.0f;
		buffer.VoxelIndex = index;
		buffer.MaterialLookup[0] = 0;
		buffer.MaterialLookup[1] = 1;
		return buffer;
	}
	//(Extents.x * Extents.y * index.z) + (Extents.x * index.y) + index.x

	void GetVoxel(IVector3 index, uint8_t* outData)
	{	

	}

	void SetVoxel(IVector3 index, uint8_t voxel)
	{
		VoxelData->SetSubData(&voxel, index, { 1, 1, 1 });
	}

	void SetVoxel(uint32_t index, uint8_t voxel)
	{
		IVector3 indexParsed{ 0 };
		indexParsed.x = glm::floor(index % Extents.x);
		indexParsed.y = glm::floor((index / Extents.x) % Extents.y);
		indexParsed.z = glm::floor(index / (Extents.x * Extents.y));
		VoxelData->SetSubData(&voxel, indexParsed, { 1, 1, 1 });
	}
};

struct Material
{
	Vector4 Albedo;
};

class RayTraceLayer;

void parallel_function(int start, int end, Chunk* layer)
{
	for (int i = start; i < end; i++)
	{
		uint8_t rand = (uint8_t)Mathf::Random::UInt32(0, 10);
		layer->SetVoxel(i, (uint8_t)(rand > 1 ? (uint8_t)255 : rand));
	}
	return;
}

class RayTraceLayer : public Layer
{
public:
	RayTraceLayer()
		: m_Camera(45.0f, 0.01f, 50.0f)
	{}

	std::vector<Chunk> Chunks;
	std::vector<Material> Materials;

	virtual void OnAttach() override
	{
		m_ViewportWidth = Application::Get().GetWindow().GetWidth();
		m_ViewportHeight = Application::Get().GetWindow().GetHeight();

		m_Image = Image::Create({ m_ViewportWidth * 2, m_ViewportHeight * 2, 0, AccessFormat::READ_WRITE, ImageFormat::RGBA8 });
		m_Compute = ComputeShader::Create("C:\\Programming\\Axton\\Axton\\internal\\shaders\\ChunkRaycaster.glsl");

		{
			Vector3 size{ 16, 16, 16 };
			Timer timer("Chunk Size Generated: " + std::to_string(size.x * size.y * size.z));

			Chunk newChunk{ Vector3(0, 0, -5), UVector3(size.x, size.y, size.z) };

			bool mt = false;

			if (!mt)
			{
				for (int z = 0; z < size.z; z++)
				{
					for (int y = 0; y < size.y; y++)
					{
						for (int x = 0; x < size.x; x++)
						{
							uint8_t rand = (uint8_t)Mathf::Random::UInt32(0, 10);
							newChunk.SetVoxel(IVector3(x, y, z), rand > 1 ? (uint8_t)255 : rand);
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
					threads.emplace_back([start, end, &newChunk]()
						{
							parallel_function(start, end, &newChunk);
						});
				}

				for (auto& thread : threads)
					thread.join();
			}

			Chunks.push_back(newChunk);
		}


		//newChunk.Position = { 0, 0, -5 };
		//Chunks.push_back(newChunk);

		//newChunk.Position = { 0, 0, 0 };
		//Chunks.push_back(newChunk);

		//newChunk.Position = { 0, 0, 5 };
		//Chunks.push_back(newChunk);

		//newChunk.Position = { 0, 0, 10 };
		//Chunks.push_back(newChunk);

		//newChunk.Position = { 0, 0, 15 };
		//Chunks.push_back(newChunk);

		//newChunk.Position = { 0, 0, 20 };
		//Chunks.push_back(newChunk);

		//newChunk.Position = { 0, 0, 25 };
		//Chunks.push_back(newChunk);

		std::vector<Chunk::Buffer> chunkBuffers;
		for (size_t i = 0; i < Chunks.size(); i++)
		{
			chunkBuffers.push_back(Chunks[i].GetBuffer(0));
		}

		Material white{ {0, 1, 1, 1} };
		Material black{ {1, 0, 0, 1} };
		Materials.push_back(white);
		Materials.push_back(black);

		m_ChunkStorage = StorageBuffer::Create(sizeof(Chunk::Buffer) * Chunks.size() , BufferUsage::DYNAM_DRAW, 2);
		m_ChunkStorage->SetData(chunkBuffers.data(), sizeof(Chunk::Buffer) * Chunks.size());

		m_MaterialStorage = StorageBuffer::Create(sizeof(Material) * Materials.size(), BufferUsage::DYNAM_DRAW, 3);
		m_MaterialStorage->SetData(Materials.data(), sizeof(Material) * Materials.size());

		std::vector<RendererID> Ids;
		for (int i = 0; i < 1; i++)
		{
			Ids.push_back(Chunks[i].VoxelData->GetRendererID());
		}

		Image3D::BindAll(1, Ids);
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

		ImGui::Begin("Camera View");
		m_ViewportWidth = (uint32_t)ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = (uint32_t)ImGui::GetContentRegionAvail().y;

		if (m_Image)
		{
			switch (renderMode)
			{
			case 0: ImGui::Image((ImTextureID)m_Image->GetRendererID(), { (float)m_Image->GetWidth() / 2, (float)m_Image->GetHeight() / 2 }, ImVec2(0, 0.5f), ImVec2(0.5f, 0)); break;
			case 1: ImGui::Image((ImTextureID)m_Image->GetRendererID(), { (float)m_Image->GetWidth() / 2, (float)m_Image->GetHeight() / 2 }, ImVec2(0.5f, 1), ImVec2(1, 0.5f)); break;
			}
		}
		ImGui::End();
	}
private:
	void TraceRays()
	{

	}

private:
	Camera m_Camera;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	Ref<Image> m_Image;
	Ref<ComputeShader> m_Compute;
	Ref<StorageBuffer> m_ChunkStorage;
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