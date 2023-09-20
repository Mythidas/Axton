#include "axpch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommands.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Axton
{
	struct QuadVertex
	{
		Vector3 Position;
		Vector4 Color;
		Vector2 TexCoord;
		float TexIndex;
	};

	struct Renderer2DData
	{
		static uint32_t const MAX_QUADS = 1000;
		static uint32_t const MAX_VERTICES = MAX_QUADS * 4;
		static uint32_t const MAX_INDICES = MAX_QUADS * 6;

		QuadVertex QuadVertices[MAX_VERTICES];
		Vector4 QuadVertexPositions[4];

		uint32_t QuadVertexCount = 0;
		uint32_t QuadIndexCount = 0;

		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> DefaultShader;

		Vector2 TexturePositions[4];

		CameraData Camera;
	};

	static Renderer2DData s_Data;
	static Renderer2DStats s_Stats;

	void Renderer2D::Construct()
	{
		s_Data.QuadVertexArray = VertexArray::Create();
		s_Data.QuadVertexArray->Bind();

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(sizeof(s_Data.QuadVertices));
		vertexBuffer->SetLayout({
			{ VertexAttrib::VAType::Float3, false },
			{ VertexAttrib::VAType::Float4, false },
			{ VertexAttrib::VAType::Float2, false },
			{ VertexAttrib::VAType::Float, false },
		});

		uint32_t indices[s_Data.MAX_INDICES];
		uint32_t index = 0;
		uint32_t offset = 0;
		while (index < s_Data.MAX_INDICES)
		{
			indices[index + 0] = offset + 0;
			indices[index + 1] = offset + 1;
			indices[index + 2] = offset + 2;

			indices[index + 3] = offset + 2;
			indices[index + 4] = offset + 3;
			indices[index + 5] = offset + 0;

			offset += 4;
			index += 6;
		}

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, s_Data.MAX_INDICES);

		s_Data.QuadVertexArray->AddBuffers(vertexBuffer, indexBuffer);
		s_Data.QuadVertexArray->Unbind();

		s_Data.QuadVertexPositions[0] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };

		s_Data.TexturePositions[0] = { 1.0f, 1.0f };
		s_Data.TexturePositions[1] = { 1.0f, 0.0f };
		s_Data.TexturePositions[2] = { 0.0f, 0.0f };
		s_Data.TexturePositions[3] = { 0.0f, 1.0f };

		// TODO: Make this not hardcoded
		s_Data.DefaultShader = Shader::Create(
			"C:/Programming/Axton/Sandbox/Assets/Shaders/SimpleShader.vert",
			"C:/Programming/Axton/Sandbox/Assets/Shaders/SimpleShader.frag", false);

		RenderCommands::SetBlendMode(true);
	}

	void Renderer2D::BeginFrame(const CameraData& camera)
	{
		s_Data.Camera = camera;
		RenderCommands::SetClearColor({ 0.0f, 0.0f, 0.1f, 1.0f });
		RenderCommands::ClearScreen();

		s_Data.DefaultShader->Bind();
		s_Data.DefaultShader->SetMat4("u_Model", Matrix4(1.0f));
		s_Data.DefaultShader->SetMat4("u_View", camera.ViewMatrix);
		s_Data.DefaultShader->SetMat4("u_Projection", camera.ProjectionMatrix);

		BeginBatch();
	}

	void Renderer2D::EndFrame()
	{
		EndBatch();
	}

	void Renderer2D::BeginBatch()
	{
		s_Data.QuadVertexCount = 0;
		s_Data.QuadIndexCount = 0;
	}

	void Renderer2D::EndBatch()
	{
		s_Data.QuadVertexArray->GetVertexBuffer()->SetData(s_Data.QuadVertices, sizeof(s_Data.QuadVertices));
		s_Data.QuadVertexArray->Bind();
		RenderCommands::DrawIndexed(s_Data.QuadIndexCount);

		s_Stats.DrawCalls++;
	}

	void Renderer2D::DrawQuad(Vector3 position, Vector2 scale, Vector4 color, Ref<Texture2D> texture)
	{
		Matrix4 transform = glm::translate(Matrix4(1.0f), position)
			* glm::scale(Matrix4(1.0f), Vector3(scale.x, scale.y, 1.0f));

		// TODO: Remove Binding
		texture->Bind();
		DrawQuad(transform, color, texture->GetRendererID());
	}

	void Renderer2D::DrawQuad(Vector3 position, Vector2 scale, Vector4 color)
	{
		Matrix4 transform = glm::translate(Matrix4(1.0f), position)
			* glm::scale(Matrix4(1.0f), Vector3(scale.x, scale.y, 1.0f));

		DrawQuad(transform, color, 0.0f);
	}

	void Renderer2D::DrawQuad(Matrix4 transform, Vector4 color, RendererID textureID)
	{
		if (s_Data.QuadVertexCount >= s_Data.MAX_VERTICES)
		{
			EndBatch();
			BeginBatch();
		}

		// TODO: Add Texture array support
		float texIndex = 0.0f;

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertices[s_Data.QuadVertexCount].Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertices[s_Data.QuadVertexCount].Color = color;
			s_Data.QuadVertices[s_Data.QuadVertexCount].TexCoord = s_Data.TexturePositions[i];
			s_Data.QuadVertices[s_Data.QuadVertexCount].TexIndex = texIndex;
			s_Data.QuadVertexCount++;
		}

		s_Data.QuadIndexCount += 6;
	}

	const Renderer2DStats Renderer2D::GetStats()
	{
		return s_Stats;
	}
}