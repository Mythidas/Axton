#pragma once

#include "Axton/Core/Defines.h"
#include "Axton/Math/Vector.h"

namespace Axton
{
	enum class GraphicsAPI
	{
		None = 0,
		Vulkan
	};

	class RenderEngine
	{
	public:
		static const int MAX_FRAMES_IN_FLIGHT = 2;

	public:
		struct Specs
		{
			GraphicsAPI API;

			Specs& setAPI(GraphicsAPI api) { API = api; return *this; }
			Scope<RenderEngine> Build(void* windowHandle) const { return Create(windowHandle, *this); }
		};

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void RenderFrame() = 0;

		static GraphicsAPI GetAPI() { return s_API; }
		static Scope<RenderEngine> Create(void* windowHandle, const Specs& specs);

	private:
		static GraphicsAPI s_API;
	};
}