#include "axpch.h"
#include "VKGraphicsContext.h"

namespace Axton::Vulkan
{
	// Validation layers enabled for debugging **TODO: Allow sandbox to set these**
	static std::vector<const char*> s_ValidationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

#ifdef AX_DEBUG
	static const bool s_EnableValidationLayers = true;
#else
	const bool EnableValidationLayers = false;
#endif

	namespace Utils
	{
		static bool CheckValidationLayerSupport()
		{
			std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

			for (auto const& name : s_ValidationLayers)
			{
				bool layerFound = false;

				for (auto const& lp : availableLayers)
				{
					if (strcmp(lp.layerName, name) == 0)
						layerFound = true;
				}

				if (!layerFound) return false;
			}

			return true;
		}

		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
			const VkDebugUtilsMessengerCallbackDataEXT*		pCallbackData,
			void*											pUserData)
		{
			CoreLog::Trace("Validation Layers: {0}", pCallbackData->pMessage);
			return VK_FALSE;
		}

		static void PopulateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT& info)
		{
			info.setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
			info.setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance);
			info.pfnUserCallback = DebugCallback;
		}

		static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback) {
			auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
			if (func != nullptr) {
				return func(instance, pCreateInfo, pAllocator, pCallback);
			}
			else {
				return VK_ERROR_EXTENSION_NOT_PRESENT;
			}
		}

		static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator) {
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
			if (func != nullptr) {
				func(instance, callback, pAllocator);
			}
		}

		static std::vector<const char*> GetRequiredExtensions()
		{
			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;
			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

			if (s_EnableValidationLayers)
			{
				extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			}

			return extensions;
		}
	}

	VKGraphicsContext::VKGraphicsContext()
		: m_Instance(nullptr), m_WindowHandle(nullptr)
	{
	}

	void VKGraphicsContext::Init(void* window)
	{
		m_WindowHandle = static_cast<GLFWwindow*>(window);

		CreateInstance();
		SetupDebugMessenger();

		VkSurfaceKHR rawSurface;
		if (glfwCreateWindowSurface(m_Instance, m_WindowHandle, nullptr, &rawSurface) != VK_SUCCESS)
		{
			AX_ASSERT_CORE(false, "Failed to create SurfaceKHR!");
		}

		m_Surface = rawSurface;
	}

	void VKGraphicsContext::SwapBuffers()
	{
		// This is controlled by the render engine
	}

	void VKGraphicsContext::CreateInstance()
	{
		if (s_EnableValidationLayers && !Utils::CheckValidationLayerSupport())
		{
			AX_ASSERT_CORE(false, "Requested Vk Validation Layers not available!");
		}

		vk::ApplicationInfo appInfo{};
		appInfo.setPApplicationName("Axton Application");
		appInfo.setApplicationVersion(VK_MAKE_VERSION(0, 1, 0));
		appInfo.setPEngineName("Axton");
		appInfo.setEngineVersion(VK_MAKE_VERSION(0, 1, 0));
		appInfo.setApiVersion(VK_API_VERSION_1_0);

		vk::InstanceCreateInfo createInfo{};
		createInfo.setPApplicationInfo(&appInfo);

		vk::DebugUtilsMessengerCreateInfoEXT debugInfo{};

		if (s_EnableValidationLayers)
		{
			createInfo.setEnabledLayerCount(static_cast<uint32_t>(s_ValidationLayers.size()));
			createInfo.setPpEnabledLayerNames(s_ValidationLayers.data());

			Utils::PopulateDebugMessengerCreateInfo(debugInfo);
			createInfo.setPNext((VkDebugUtilsMessengerCreateInfoEXT*)&debugInfo);
		}

		std::vector<const char*> glfwExtensions = Utils::GetRequiredExtensions();

		createInfo.setEnabledExtensionCount(static_cast<uint32_t>(glfwExtensions.size()));
		createInfo.setPpEnabledExtensionNames(glfwExtensions.data());

		if (vk::createInstance(&createInfo, nullptr, &m_Instance) != vk::Result::eSuccess)
		{
			AX_ASSERT_CORE(false, "Failed to create VkInstance!");
		}

		std::vector<vk::ExtensionProperties> extensions = vk::enumerateInstanceExtensionProperties();

		CoreLog::Trace("Available Extensions: ");

		for (auto const& ext : extensions)
			CoreLog::Trace("\t{0}", ext.extensionName);
	}

	void VKGraphicsContext::SetupDebugMessenger()
	{
		if (!s_EnableValidationLayers) return;

		vk::DebugUtilsMessengerCreateInfoEXT createInfo{};
		Utils::PopulateDebugMessengerCreateInfo(createInfo);

		if (Utils::CreateDebugUtilsMessengerEXT(m_Instance, reinterpret_cast<const VkDebugUtilsMessengerCreateInfoEXT*>(&createInfo), nullptr, &m_DebugMessenger) != VK_SUCCESS) {
			AX_ASSERT_CORE(false, "Failed to create DebugCallback!");
		}
	}
}