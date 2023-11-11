#pragma once

#include "Axton/Core/Defines.h"
#include "Axton/Math/Vector.h"

namespace Axton
{
	class RenderEngine
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
		static Scope<RenderEngine> Create(void* windowHandle);

	private:
		static API s_API;
	};
}