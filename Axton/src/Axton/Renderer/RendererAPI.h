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
			Vulkan
		};

		static const int MAX_FRAMES_IN_FLIGHT = 2;

	public:
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void OnUpdate() = 0;

		static API GetAPI() { return s_API; }

		static Scope<RendererAPI> Create(void* windowHandle);

	protected:

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