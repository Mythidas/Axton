#pragma once

#include "Axton/Renderer/GraphicsContext.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

namespace Axton::Vulkan
{
	class VKGraphicsContext : public Axton::GraphicsContext
	{
	public:
		VKGraphicsContext();

		virtual void Init(void* window) override;
		virtual void SwapBuffers() override;

		vk::Instance& GetVkInstance() { return m_Instance; }
		vk::SurfaceKHR& GetVkSurface() { return m_Surface; }

	private:
		GLFWwindow* m_WindowHandle;

		vk::Instance m_Instance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		vk::SurfaceKHR m_Surface;

	private:
		void CreateInstance();
		void SetupDebugMessenger();
	};
}