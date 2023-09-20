#pragma once

#include "Texture2D.h"
#include "Axton/Math/Math.h"

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

		static void BeginFrame();
		static void EndFrame();

		static void DrawQuad(Vector3 position, Vector2 scale, Vector4 color, Ref<Texture2D> texture);
		static void DrawQuad(Vector3 position, Vector2 scale, Vector4 color);
		static void DrawQuad(Matrix4 transform, Vector4 color, RendererID textureID);

		static const Renderer2DStats GetStats();

	private:
		static void BeginBatch();
		static void EndBatch();
	};
}