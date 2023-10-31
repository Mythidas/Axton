#pragma once

#include "Axton/Core/Defines.h"
#include "Axton/Math/Vector.h"

namespace Axton
{
	typedef unsigned int RendererID;

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL,
			Vulkan
		};

	public:
		virtual void ClearScreen() = 0;
		virtual void SetClearColor(const Vector4& clearColor) = 0;
		virtual void SetBlendMode(bool enabled) = 0;
		virtual void SetViewport(int& width, int& height) = 0;
		virtual void EnableDepthTest(bool enabled) = 0;

		virtual void DrawIndexed(uint32_t count) = 0;

		static API GetAPI() { return s_API; }
		static Scope<RendererAPI> Create();
	private:
		static API s_API;
	};

	class ImageFormat
	{
	public:
		enum class Data
		{
			None = 0,
			R8,
			RGB8,
			RGBA8,
			RGBA32F
		};

		enum class Filter
		{
			NEAREST,
			LINEAR
		};

		enum class Wrap
		{
			CLAMP,
			REPEAT
		};

		enum class Access
		{
			READ_WRITE,
			READ_ONLY,
			WRITE_ONLY
		};
	};
}