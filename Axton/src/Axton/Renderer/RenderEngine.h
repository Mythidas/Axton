#pragma once

#include "Axton/Core/Defines.h"
#include "Axton/Math/Vector.h"

namespace Axton
{
	enum class RenderAPI
	{
		None = 0,
		Vulkan
	};

	enum class RenderExtension // Need to rewrite some vulkan code to use these
	{
		Graphics	= 1 << 0,
		Compute		= 1 << 1,
		RayTracing	= 1 << 2
	};

	RenderExtension operator |(RenderExtension lhs, RenderExtension rhs);
	bool operator &(RenderExtension lhs, RenderExtension rhs);

	class RenderEngine
	{
	public:
		static const int MAX_FRAMES_IN_FLIGHT = 2;

	public:
		struct Specs
		{
			RenderAPI API;
			RenderExtension Extensions;

			Specs& setAPI(RenderAPI api) { API = api; return *this; }
			Specs& setExtensions(RenderExtension extensions) { Extensions = extensions; return *this; }
			Scope<RenderEngine> Build(void* windowHandle) const { return Create(windowHandle, *this); }
		};

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void RenderFrame() = 0;

		static RenderAPI GetAPI() { return s_API; }
		static Scope<RenderEngine> Create(void* windowHandle, const Specs& specs);

	private:
		static RenderAPI s_API;
	};
}