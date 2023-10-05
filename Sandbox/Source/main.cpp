#include "Axton.h"
#include "Axton/Core/EntryPoint.h"

#include "Camera.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

using namespace Axton;

struct Voxel
{
	uint32_t Color;

	void PackColor(const Vector4& color)
	{
		Color |= ((uint32_t)(color.a * 255.0f) << 24) | ((uint32_t)(color.b * 255.0f) << 16) | 
			((uint32_t)(color.g * 255.0f) << 8) | ((uint32_t)(color.r * 255.0f));
	}

	Vector4 UnpackColor()
	{
		return Vector4(1);
	}
};

struct Chunk
{
	std::vector<Voxel> Voxels;
	uint32_t Size;

	Chunk(uint32_t size)
		: Size(size)
	{
		Voxel black{};
		black.PackColor({ 0, 0, 0, 0 });
		Voxels.resize(size * size * size, black);
	}

	Voxel GetVoxel(uint32_t x, uint32_t y, uint32_t z)
	{
		return Voxels[x + Size * (y + Size * z)];
	}

	void SetVoxel(uint32_t x, uint32_t y, uint32_t z, Voxel voxel)
	{
		Voxels[x + Size * (y + Size * z)] = voxel;
	}
};

class RayTraceLayer : public Layer
{
public:
	RayTraceLayer()
		: m_Camera(45.0f, 0.01f, 50.0f)
	{}

	Chunk newChunk{ 4 };

	virtual void OnAttach() override
	{
		m_ViewportWidth = Application::Get().GetWindow().GetWidth();
		m_ViewportHeight = Application::Get().GetWindow().GetHeight();

		m_Image = Image::Create({ m_ViewportWidth, m_ViewportHeight, 0, AccessFormat::READ_WRITE, ImageFormat::RGBA8 });
		m_Compute = ComputeShader::Create("C:\\Programming\\Axton\\Axton\\internal\\shaders\\ChunkRaycaster.glsl");

		Voxel white{};
		white.PackColor({ 1, 1, 1, 1 });
		newChunk.SetVoxel(0, 0, 0, white);
		newChunk.SetVoxel(1, 0, 0, white);
		newChunk.SetVoxel(1, 1, 0, white);
		newChunk.SetVoxel(1, 2, 0, white);
		newChunk.SetVoxel(1, 3, 0, white);

		m_VoxelStorage = StorageBuffer::Create(sizeof(Voxel) * newChunk.Voxels.size(), BufferUsage::DYNAM_COPY, 2);
		m_VoxelStorage->SetData(newChunk.Voxels.data(), sizeof(Voxel) * newChunk.Voxels.size());
	}

	virtual void OnUpdate() override
	{
		m_Camera.OnUpdate();
		m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);

		m_Image->Resize(m_ViewportWidth, m_ViewportHeight);

		m_Compute->Dispatch(m_ViewportWidth, m_ViewportHeight, 1);
		m_Compute->Barrier();
	}

	virtual void OnRenderUI() override
	{
		Vector3 cam = m_Camera.GetPosition();
		ImGui::Begin("Render Stats");
		ImGui::Text("FPS: %f", 1.0f / Time::GetDeltaTime());
		ImGui::Text("Render Time: %f ms", Time::GetDeltaTime());
		ImGui::End();

		ImGui::Begin("Camera View");
		m_ViewportWidth = (uint32_t)ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = (uint32_t)ImGui::GetContentRegionAvail().y;

		if (m_Image)
		{
			ImGui::Image((ImTextureID)m_Image->GetRendererID(), { (float)m_Image->GetWidth(), (float)m_Image->GetHeight() }, ImVec2(0, 1), ImVec2(1, 0));
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
	Ref<StorageBuffer> m_VoxelStorage;
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