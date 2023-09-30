#pragma once

#include "Texture2D.h"
#include "Sprite.h"
#include "CameraSpecs.h"
#include "Axton/Math/Math.h"
#include "Axton/Core/Defines.h"

namespace Axton
{
	struct Renderer2DStats
	{
		uint32_t DrawCalls = 0;
	};

	class Renderer2D
	{
	public:
		static void Construct();
		static void Destruct();

		static void BeginFrame(const CameraSpecs& camera);
		static void EndFrame();

		static void DrawQuad(Vector3 position, Vector2 scale, Vector4 color, Ref<Sprite> sprite);
		static void DrawQuad(Vector3 position, Vector2 scale, Vector4 color, Ref<Texture2D> texture);
		static void DrawQuad(Vector3 position, Vector2 scale, Vector4 color);
		static void DrawQuad(Matrix4 transform, Vector4 color, Ref<Texture2D> texture);

		static void DrawRotateQuad(Vector3 position, Vector3 rotation, Vector2 scale, Vector4 color, Ref<Sprite> sprite);
		static void DrawRotateQuad(Vector3 position, Vector3 rotation, Vector2 scale, Vector4 color);
		static void DrawRotateQuad(Matrix4 transform, Vector4 color, Ref<Sprite> sprite);

		static void DrawQuadUI(Vector2 position, Vector2 scale, Vector4 color, Ref<Texture2D> texture);
		static void DrawQuadUI(Matrix4 transform, Vector4 color, Ref<Texture2D> texture);
		static void DrawQuadUI(Matrix4 transform, Vector4 color, Ref<Sprite> sprite);

		static void DrawRotateQuadUI(Vector2 position, float rotation, Vector2 scale, Vector4 color, Ref<Texture2D> texture);
		static void DrawRotateQuadUI(Vector2 position, float rotation, Vector2 scale, Vector4 color, Ref<Sprite> sprite);

	private:
		static void BeginBatch();
		static void EndBatch();

		static void CheckBatch();

		static float GetQuadTextureIndex(Ref<Texture2D> texture);
		static float GetUIQuadTextureIndex(Ref<Texture2D> texture);
	};
}