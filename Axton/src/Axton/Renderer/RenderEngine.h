#pragma once

#include "Axton/Core/Defines.h"
#include "Swapchain.h"
#include "RenderPass.h"

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

		static void Construct();
		static void Destruct();

		static void BeginBatch();
		static void EndBatch();

		static RenderAPI GetAPI() { return s_API; }
		static Ref<Swapchain>& GetSwapchain() { return s_Swapchain; }
		static Ref<RenderPass>& GetRenderPass() { return s_RenderPass; }

		static Scope<RenderEngine> Create(void* windowHandle, const Specs& specs);

	public:
		virtual void RenderFrame() = 0;

	private:
		static RenderAPI s_API;

		static Ref<Swapchain> s_Swapchain;
		static Ref<RenderPass> s_RenderPass;
	};
}