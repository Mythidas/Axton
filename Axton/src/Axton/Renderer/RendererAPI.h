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
		static void Construct();

		static API GetAPI() { return s_API; }

		static void ClearScreen();
		static void SetClearColor(const Vector4& clearColor);
		static void SetBlendMode(bool enabled);
		static void SetViewport(int& width, int& height);
		static void EnableDepthTest(bool enabled);
		static void DrawIndexed(uint32_t count);

	protected:
		virtual void clearScreen() = 0;
		virtual void setClearColor(const Vector4& clearColor) = 0;
		virtual void setBlendMode(bool enabled) = 0;
		virtual void setViewport(int& width, int& height) = 0;
		virtual void enableDepthTest(bool enabled) = 0;

		virtual void drawIndexed(uint32_t count) = 0;

	private:
		static API s_API;
		static Scope<RendererAPI> s_Instance;
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