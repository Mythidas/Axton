#include "axpch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommands.h"
#include "UniformBuffer.h"

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

	struct UIQuadVertex
	{
		Vector2 Position;
		Vector4 Color;
		Vector2 TexCoord;
		float TexIndex;
	};

	struct Renderer2DData
	{
		static int const MAX_QUADS = 1000;
		static int const MAX_VERTICES = MAX_QUADS * 4;
		static int const MAX_INDICES = MAX_QUADS * 6;
		static int const MAX_TEXTURES = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> QuadShader;

		Ref<VertexArray> UIQuadVertexArray;
		Ref<Shader> UIQuadShader;

		int QuadIndexCount = 0;
		QuadVertex* QuadVertexBuffer = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		int UIQuadIndexCount = 0;
		UIQuadVertex* UIQuadVertexBuffer = nullptr;
		UIQuadVertex* UIQuadVertexBufferPtr = nullptr;

		Vector4 QuadVertexPositions[4];

		std::array<Ref<Texture2D>, MAX_TEXTURES> QuadTextureSlots;
		int QuadTextureSlotIndex = 1;


		std::array<Ref<Texture2D>, MAX_TEXTURES> UIQuadTextureSlots;
		int UIQuadTextureSlotIndex = 1;

		Vector2 TexturePositions[4];

		struct CameraBuffer
		{
			Matrix4 ViewProjection;
		};

		CameraBuffer CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
	};

	static Renderer2DData s_Data2D;

	void Renderer2D::Construct()
	{
		s_Data2D.QuadVertexBuffer = new QuadVertex[s_Data2D.MAX_VERTICES];
		s_Data2D.UIQuadVertexBuffer = new UIQuadVertex[s_Data2D.MAX_VERTICES];

		// Index Buffer
		uint32_t* indices = new uint32_t[s_Data2D.MAX_INDICES];
		int index = 0;
		int offset = 0;
		while (index < s_Data2D.MAX_INDICES)
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

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, s_Data2D.MAX_INDICES);
		delete[] indices;

		// Quad Vertex Array
		s_Data2D.QuadVertexArray = VertexArray::Create();
		s_Data2D.QuadVertexArray->Bind();

		Ref<VertexBuffer> quadVertexBuffer = VertexBuffer::Create(s_Data2D.MAX_VERTICES * sizeof(QuadVertex));
		quadVertexBuffer->SetLayout({
			{ VertexAttrib::VAType::Float3, false },
			{ VertexAttrib::VAType::Float4, false },
			{ VertexAttrib::VAType::Float2, false },
			{ VertexAttrib::VAType::Float, false },
		});

		s_Data2D.QuadVertexArray->AddBuffers(quadVertexBuffer, indexBuffer);
		s_Data2D.QuadVertexArray->Unbind();

		// UI Quad Vertex Array
		s_Data2D.UIQuadVertexArray = VertexArray::Create();
		s_Data2D.UIQuadVertexArray->Bind();

		Ref<VertexBuffer> uiQuadVertexBuffer = VertexBuffer::Create(s_Data2D.MAX_VERTICES * sizeof(UIQuadVertex));
		uiQuadVertexBuffer->SetLayout({
			{ VertexAttrib::VAType::Float2, false },
			{ VertexAttrib::VAType::Float4, false },
			{ VertexAttrib::VAType::Float2, false },
			{ VertexAttrib::VAType::Float, false },
			});

		s_Data2D.UIQuadVertexArray->AddBuffers(uiQuadVertexBuffer, indexBuffer);
		s_Data2D.UIQuadVertexArray->Unbind();

		s_Data2D.QuadVertexPositions[0] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_Data2D.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data2D.QuadVertexPositions[2] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data2D.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };

		s_Data2D.TexturePositions[0] = { 1.0f, 1.0f };
		s_Data2D.TexturePositions[1] = { 1.0f, 0.0f };
		s_Data2D.TexturePositions[2] = { 0.0f, 0.0f };
		s_Data2D.TexturePositions[3] = { 0.0f, 1.0f };

		uint32_t whiteTextureData = 0xff5082d3;
		Ref<Texture2D> whiteTexture = Texture2D::Create(Texture2DSpecs());
		whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		s_Data2D.QuadTextureSlots[0] = whiteTexture;
		s_Data2D.UIQuadTextureSlots[0] = whiteTexture;

		// TODO: Make this not hardcoded
		s_Data2D.QuadShader = Shader::Create(
			"C:/Programming/Axton/Sandbox/Assets/Shaders/QuadShader.vert",
			"C:/Programming/Axton/Sandbox/Assets/Shaders/QuadShader.frag", false);

		s_Data2D.UIQuadShader = Shader::Create(
			"C:/Programming/Axton/Sandbox/Assets/Shaders/UIQuadShader.vert",
			"C:/Programming/Axton/Sandbox/Assets/Shaders/UIQuadShader.frag", false);

		s_Data2D.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraBuffer), 0);

		RenderCommands::SetBlendMode(true);
	}

	void Renderer2D::Destruct()
	{
		delete[] s_Data2D.QuadVertexBuffer;
		delete[] s_Data2D.UIQuadVertexBuffer;
	}

	void Renderer2D::BeginFrame(const CameraData& camera)
	{
		s_Data2D.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();

		s_Data2D.CameraUniformBuffer->SetData(&s_Data2D.CameraBuffer, sizeof(Renderer2DData::CameraBuffer));

		BeginBatch();
	}

	void Renderer2D::EndFrame()
	{
		EndBatch();
	}

	void Renderer2D::BeginBatch()
	{
		s_Data2D.QuadIndexCount = 0;
		s_Data2D.QuadTextureSlotIndex = 1;

		s_Data2D.QuadVertexBufferPtr = s_Data2D.QuadVertexBuffer;

		s_Data2D.UIQuadIndexCount = 0;
		s_Data2D.UIQuadTextureSlotIndex = 1;

		s_Data2D.UIQuadVertexBufferPtr = s_Data2D.UIQuadVertexBuffer;
	}

	void Renderer2D::EndBatch()
	{
		if (s_Data2D.QuadIndexCount)
		{
			for (uint32_t i = 0; i < s_Data2D.QuadTextureSlotIndex; i++)
			{
				s_Data2D.QuadTextureSlots[i]->Bind(i);
			}

			signed long long size = (uint8_t*)s_Data2D.QuadVertexBufferPtr - (uint8_t*)s_Data2D.QuadVertexBuffer;
			s_Data2D.QuadVertexArray->GetVertexBuffer()->SetData(s_Data2D.QuadVertexBuffer, size);

			s_Data2D.QuadShader->Bind();
			s_Data2D.QuadVertexArray->Bind();
			RenderCommands::DrawIndexed(s_Data2D.QuadIndexCount);
		}

		if (s_Data2D.UIQuadIndexCount)
		{
			for (uint32_t i = 0; i < s_Data2D.UIQuadTextureSlotIndex; i++)
			{
				s_Data2D.UIQuadTextureSlots[i]->Bind(i);
			}

			signed long long size = (uint8_t*)s_Data2D.UIQuadVertexBufferPtr - (uint8_t*)s_Data2D.UIQuadVertexBuffer;
			s_Data2D.UIQuadVertexArray->GetVertexBuffer()->SetData(s_Data2D.UIQuadVertexBuffer, size);

			s_Data2D.UIQuadVertexArray->Bind();
			s_Data2D.UIQuadShader->Bind();
			RenderCommands::DrawIndexed(s_Data2D.UIQuadIndexCount);
		}
	}

	void Renderer2D::DrawQuad(Vector3 position, Vector2 scale, Vector4 color, Ref<Texture2D> texture)
	{
		Matrix4 transform = glm::translate(Matrix4(1.0f), position)
			* glm::scale(Matrix4(1.0f), Vector3(scale.x, scale.y, 1.0f));

		DrawQuad(transform, color, texture);
	}

	void Renderer2D::DrawQuad(Vector3 position, Vector2 scale, Vector4 color)
	{
		Matrix4 transform = glm::translate(Matrix4(1.0f), position)
			* glm::scale(Matrix4(1.0f), Vector3(scale.x, scale.y, 1.0f));

		DrawQuad(transform, color, nullptr);
	}

	void Renderer2D::DrawQuad(Matrix4 transform, Vector4 color, Ref<Texture2D> texture)
	{
		CheckBatch();
		float texIndex = GetQuadTextureIndex(texture);

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data2D.QuadVertexBufferPtr->Position = transform * s_Data2D.QuadVertexPositions[i];
			s_Data2D.QuadVertexBufferPtr->Color = color;
			s_Data2D.QuadVertexBufferPtr->TexCoord = s_Data2D.TexturePositions[i];
			s_Data2D.QuadVertexBufferPtr->TexIndex = texIndex;
			s_Data2D.QuadVertexBufferPtr++;
		}

		s_Data2D.QuadIndexCount += 6;
	}

	void Renderer2D::DrawRotateQuad(Vector3 position, Vector3 rotation, Vector2 scale, Vector4 color, Ref<Sprite> sprite)
	{
		Matrix4 transform = glm::translate(Matrix4(1.0f), position)
			* glm::rotate(Matrix4(1.0f), rotation.x, Vector3(1.0f, 0.0f, 0.0f))
			* glm::rotate(Matrix4(1.0f), rotation.y, Vector3(0.0f, 1.0f, 0.0f))
			* glm::rotate(Matrix4(1.0f), rotation.z, Vector3(0.0f, 0.0f, 1.0f))
			* glm::scale(Matrix4(1.0f), Vector3(scale.x, scale.y, 1.0f));

		DrawRotateQuad(transform, color, sprite);
	}

	void Renderer2D::DrawRotateQuad(Vector3 position, Vector3 rotation, Vector2 scale, Vector4 color)
	{
		Matrix4 transform = glm::translate(Matrix4(1.0f), position)
			* glm::rotate(Matrix4(1.0f), rotation.x, Vector3(1.0f, 0.0f, 0.0f))
			* glm::rotate(Matrix4(1.0f), rotation.y, Vector3(0.0f, 1.0f, 0.0f))
			* glm::rotate(Matrix4(1.0f), rotation.z, Vector3(0.0f, 0.0f, 1.0f))
			* glm::scale(Matrix4(1.0f), Vector3(scale.x, scale.y, 1.0f));

		DrawQuad(transform, color, nullptr);
	}

	void Renderer2D::DrawRotateQuad(Matrix4 transform, Vector4 color, Ref<Sprite> sprite)
	{
		CheckBatch();
		float texIndex = GetQuadTextureIndex(sprite->GetTexture());

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data2D.QuadVertexBufferPtr->Position = transform * s_Data2D.QuadVertexPositions[i];
			s_Data2D.QuadVertexBufferPtr->Color = color;
			s_Data2D.QuadVertexBufferPtr->TexCoord = sprite->GetTexCoords()[i];
			s_Data2D.QuadVertexBufferPtr->TexIndex = texIndex;
			s_Data2D.QuadVertexBufferPtr++;
		}

		s_Data2D.QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuadUI(Vector2 position, Vector2 scale, Vector4 color, Ref<Texture2D> texture)
	{
		Matrix4 transform = glm::translate(Matrix4(1.0f), Vector3(position.x, position.y, 0.0f))
			* glm::scale(Matrix4(1.0f), Vector3(scale.x, scale.y, 1.0f));

		DrawQuadUI(transform, color, texture);
	}

	void Renderer2D::DrawQuadUI(Matrix4 transform, Vector4 color, Ref<Texture2D> texture)
	{
		CheckBatch();
		float texIndex = GetUIQuadTextureIndex(texture);

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data2D.UIQuadVertexBufferPtr->Position = transform * s_Data2D.QuadVertexPositions[i];
			s_Data2D.UIQuadVertexBufferPtr->Color = color;
			s_Data2D.UIQuadVertexBufferPtr->TexCoord = s_Data2D.TexturePositions[i];
			s_Data2D.UIQuadVertexBufferPtr->TexIndex = texIndex;
			s_Data2D.UIQuadVertexBufferPtr++;
		}

		s_Data2D.UIQuadIndexCount += 6;
	}

	void Renderer2D::DrawQuadUI(Matrix4 transform, Vector4 color, Ref<Sprite> sprite)
	{
		CheckBatch();
		float texIndex = 0.0f;
		if (sprite)
		{
			texIndex = GetUIQuadTextureIndex(sprite->GetTexture());
		}

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data2D.UIQuadVertexBufferPtr->Position = transform * s_Data2D.QuadVertexPositions[i];
			s_Data2D.UIQuadVertexBufferPtr->Color = color;
			s_Data2D.UIQuadVertexBufferPtr->TexCoord = sprite->GetTexCoords()[i];
			s_Data2D.UIQuadVertexBufferPtr->TexIndex = texIndex;
			s_Data2D.UIQuadVertexBufferPtr++;
		}

		s_Data2D.UIQuadIndexCount += 6;
	}

	void Renderer2D::DrawRotateQuadUI(Vector2 position, float rotation, Vector2 scale, Vector4 color, Ref<Texture2D> texture)
	{
		Matrix4 transform = glm::translate(Matrix4(1.0f), Vector3(position.x, position.y, 0.0f))
			* glm::rotate(Matrix4(1.0f), rotation, Vector3(0.0f, 0.0f, 1.0f))
			* glm::scale(Matrix4(1.0f), Vector3(scale.x, scale.y, 1.0f));

		DrawQuadUI(transform, color, texture);
	}

	void Renderer2D::DrawRotateQuadUI(Vector2 position, float rotation, Vector2 scale, Vector4 color, Ref<Sprite> sprite)
	{
		Matrix4 transform = glm::translate(Matrix4(1.0f), Vector3(position.x, position.y, 0.0f))
			* glm::rotate(Matrix4(1.0f), rotation, Vector3(0.0f, 0.0f, 1.0f))
			* glm::scale(Matrix4(1.0f), Vector3(scale.x, scale.y, 1.0f));

		DrawQuadUI(transform, color, sprite);
	}

	void Renderer2D::CheckBatch()
	{
		if (s_Data2D.QuadIndexCount >= s_Data2D.MAX_INDICES || s_Data2D.QuadTextureSlotIndex == s_Data2D.MAX_TEXTURES
			|| s_Data2D.UIQuadIndexCount >= s_Data2D.MAX_INDICES || s_Data2D.UIQuadTextureSlotIndex == s_Data2D.MAX_TEXTURES)
		{
			EndBatch();
			BeginBatch();
		}
	}

	float Renderer2D::GetQuadTextureIndex(Ref<Texture2D> texture)
	{
		float texIndex = 0.0f;
		if (texture)
		{
			for (uint32_t i = 1; i < s_Data2D.QuadTextureSlotIndex; i++)
			{
				if (s_Data2D.QuadTextureSlots[i]->GetRendererID() == texture->GetRendererID())
				{
					texIndex = (float)i;
					break;
				}
			}

			if (texIndex == 0)
			{
				texIndex = (float)s_Data2D.QuadTextureSlotIndex;
				s_Data2D.QuadTextureSlots[texIndex] = texture;
				s_Data2D.QuadTextureSlotIndex++;
			}
		}

		return texIndex;
	}

	float Renderer2D::GetUIQuadTextureIndex(Ref<Texture2D> texture)
	{
		float texIndex = 0.0f;
		if (texture)
		{
			for (uint32_t i = 1; i < s_Data2D.UIQuadTextureSlotIndex; i++)
			{
				if (s_Data2D.UIQuadTextureSlots[i]->GetRendererID() == texture->GetRendererID())
				{
					texIndex = (float)i;
					break;
				}
			}

			if (texIndex == 0)
			{
				texIndex = (float)s_Data2D.UIQuadTextureSlotIndex;
				s_Data2D.UIQuadTextureSlots[texIndex] = texture;
				s_Data2D.UIQuadTextureSlotIndex++;
			}
		}

		return texIndex;
	}
}