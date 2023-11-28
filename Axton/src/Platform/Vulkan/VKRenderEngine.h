#pragma once
#include "VKGraphicsContext.h"
#include "Axton/Renderer/RenderEngine.h"

#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKRenderEngine : public RenderEngine
	{
	public:
		VKRenderEngine(void* windowHandle, const Specs& specs);
		~VKRenderEngine();

		virtual void RenderFrame() override;

		static vk::CommandBuffer& GetCommandBuffer() { return s_Singleton->m_GraphicsContext->GetCommandBuffer(); }
		static vk::Instance& GetInstance() { return s_Singleton->m_GraphicsContext->GetInstance(); }
		static vk::SurfaceKHR& GetSurface() { return s_Singleton->m_GraphicsContext->GetSurface(); }
		static vk::PhysicalDevice& GetPhysicalDevice() { return s_Singleton->m_GraphicsContext->GetPhysicalDevice(); }
		static vk::Device& GetDevice() { return s_Singleton->m_GraphicsContext->GetDevice(); }
		static uint32_t GetCurrentFrame() { return s_Singleton->m_GraphicsContext->GetCurrentFrame(); }
		static Ref<VKGraphicsContext> GetGraphicsContext() { return s_Singleton->m_GraphicsContext; }

	private:
		bool onWindowResized(int width, int height);

	private:
		static VKRenderEngine* s_Singleton;

		Specs m_Specs;
		Ref<VKGraphicsContext> m_GraphicsContext;

		std::vector<vk::Semaphore> m_ImageAvailable;
		std::vector<vk::Semaphore> m_RenderFinished;
		std::vector<vk::Fence> m_InFlight;

		bool m_FramebufferInvalid = false;
		bool m_FramebufferResized = false;
	};
}