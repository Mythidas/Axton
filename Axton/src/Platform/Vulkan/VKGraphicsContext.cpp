#include "axpch.h"
#include "VKGraphicsContext.h"
#include "Platform/Vulkan/VKRenderEngine.h"

#include <GLFW/glfw3.h>

namespace Axton::Vulkan
{
	namespace Utils
	{
		bool checkValidationSupport(const std::vector<const char*> validationLayers)
		{
			std::vector<vk::LayerProperties> properties = vk::enumerateInstanceLayerProperties();

			for (const char* layer : validationLayers)
			{
				bool layerFound = false;

				for (const auto& property : properties)
				{
					if (strcmp(layer, property.layerName))
					{
						layerFound = true;
						break;
					}
				}

				if (!layerFound)
					return false;
			}

			return true;
		}

		VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messaageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData
		)
		{
			CoreLog::Trace("Validation Layer: {0}\n", pCallbackData->pMessage);
			return VK_FALSE;
		}

		vk::Result createDebugUtilsMessenger(vk::Instance instance, const vk::DebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const vk::AllocationCallbacks* pAllocator, vk::DebugUtilsMessengerEXT* pDebugMessenger)
		{
			auto func = (PFN_vkCreateDebugUtilsMessengerEXT)instance.getProcAddr("vkCreateDebugUtilsMessengerEXT");
			if (func)
			{
				return vk::Result(func(instance, reinterpret_cast<const VkDebugUtilsMessengerCreateInfoEXT*>(pCreateInfo), nullptr, reinterpret_cast<VkDebugUtilsMessengerEXT*>(pDebugMessenger)));
			}
			else
			{
				return vk::Result::eErrorExtensionNotPresent;
			}
		}

		void destroyDebugUtilsMessenger(vk::Instance instance, vk::DebugUtilsMessengerEXT debugMessenger)
		{
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT");
			if (func)
			{
				func(instance, debugMessenger, nullptr);
			}
		}

		bool isDeviceSuitable(vk::PhysicalDevice pDevice, vk::SurfaceKHR surface, const std::vector<const char*> deviceExtensions)
		{
			std::vector<vk::ExtensionProperties> extProperties = pDevice.enumerateDeviceExtensionProperties();
			std::unordered_set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

			std::vector<vk::SurfaceFormatKHR> surfaceFormats = pDevice.getSurfaceFormatsKHR(surface);
			std::vector<vk::PresentModeKHR> presentModes = pDevice.getSurfacePresentModesKHR(surface);

			for (const auto& extension : extProperties)
			{
				requiredExtensions.erase(extension.extensionName);
			}

			bool swapChainAdequate = false;
			if (requiredExtensions.empty()) {
				swapChainAdequate = !surfaceFormats.empty() && !presentModes.empty();
			}

			vk::PhysicalDeviceProperties properties = pDevice.getProperties();
			return properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu && requiredExtensions.empty() && swapChainAdequate;
		}
	}

	Ref<VKGraphicsContext> VKGraphicsContext::Create(void* windowHandle, const std::vector<const char*> deviceExtensions, const std::vector<const char*> validationLayers)
	{
		Ref<VKGraphicsContext> context = CreateRef<VKGraphicsContext>();

		context->createInstance(validationLayers);

		if (!validationLayers.empty())
		{
			context->createDebugMessenger();
		}

		context->createSurfaceKHR(windowHandle);
		context->createPhysicalDevice(deviceExtensions);
		context->createLogicalDevice(deviceExtensions, validationLayers);
		context->createCommandPool();

		return context;
	}

	void VKGraphicsContext::Update()
	{
		m_CurrentFrame = (m_CurrentFrame + 1) % VKRenderEngine::MAX_FRAMES_IN_FLIGHT;
	}

	void VKGraphicsContext::Destroy()
	{
		while (!m_DeletionQueue.Empty())
		{
			m_DeletionQueue.Deque()();
		}

		m_Device.destroy(m_CommandPool);
		m_Device.destroy();

		Utils::destroyDebugUtilsMessenger(m_Instance, m_Debug);
		m_Instance.destroySurfaceKHR(m_Surface);
		m_Instance.destroy();
	}

	void VKGraphicsContext::QueueDeletion(std::function<void()> func)
	{
		m_DeletionQueue.Enque(func);
	}

	void VKGraphicsContext::SubmitCommand(std::function<void(vk::CommandBuffer&)> func)
	{
		vk::CommandBufferAllocateInfo allocInfo{};
		allocInfo
			.setLevel(vk::CommandBufferLevel::ePrimary)
			.setCommandPool(m_CommandPool)
			.setCommandBufferCount(1);

		vk::CommandBufferBeginInfo beginInfo{};
		beginInfo
			.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

		vk::CommandBuffer buffer = m_Device.allocateCommandBuffers(allocInfo)[0];

		buffer.begin(beginInfo);

		func(buffer);

		buffer.end();

		vk::SubmitInfo submitInfo{};
		submitInfo
			.setCommandBufferCount(1)
			.setPCommandBuffers(&buffer);

		// May be an issue if a Different queue is needed
		m_GraphicsQueue.submit({ submitInfo }, VK_NULL_HANDLE);
		m_GraphicsQueue.waitIdle();

		m_Device.freeCommandBuffers(m_CommandPool, buffer);
	}

	void VKGraphicsContext::SubmitGraphicsQueue(const QueueSubmitInfo& queueSubmitInfo)
	{
		vk::SubmitInfo submitInfo{};
		submitInfo
			.setWaitSemaphoreCount(static_cast<uint32_t>(queueSubmitInfo.WaitSemaphores.size()))
			.setPWaitSemaphores(queueSubmitInfo.WaitSemaphores.data())
			.setPWaitDstStageMask(queueSubmitInfo.WaitStages.data())
			.setCommandBufferCount(1)
			.setPCommandBuffers(&m_CommandBuffers[m_CurrentFrame])
			.setSignalSemaphoreCount(static_cast<uint32_t>(queueSubmitInfo.SignalSemaphores.size()))
			.setPSignalSemaphores(queueSubmitInfo.SignalSemaphores.data());

		m_GraphicsQueue.submit(submitInfo, queueSubmitInfo.Fence);
	}

	vk::Result VKGraphicsContext::SubmitPresentQueue(const QueueSubmitInfo& queueSubmitInfo, const std::vector<vk::SwapchainKHR>& swapchains, uint32_t imageIndex)
	{
		vk::PresentInfoKHR presentInfo{};
		presentInfo
			.setWaitSemaphoreCount(static_cast<uint32_t>(queueSubmitInfo.WaitSemaphores.size()))
			.setPWaitSemaphores(queueSubmitInfo.WaitSemaphores.data())
			.setSwapchainCount(static_cast<uint32_t>(swapchains.size()))
			.setPSwapchains(swapchains.data())
			.setPImageIndices(&imageIndex);

		return m_PresentQueue.presentKHR(presentInfo);
	}

	void VKGraphicsContext::createInstance(const std::vector<const char*> validationLayers)
	{
		vk::ApplicationInfo appInfo{};
		appInfo
			.setPApplicationName("AxtonVulkan")
			.setApplicationVersion(VK_MAKE_VERSION(0, 1, 0))
			.setPEngineName("AxtonEngine")
			.setEngineVersion(VK_MAKE_VERSION(0, 1, 0))
			.setApiVersion(VK_API_VERSION_1_0);

		vk::InstanceCreateInfo createInfo{};
		createInfo
			.setPApplicationInfo(&appInfo);

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (!validationLayers.empty())
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

			if (!Utils::checkValidationSupport(validationLayers))
				AX_ASSERT_CORE("Validation layers requested, but not available!");

			createInfo
				.setEnabledLayerCount(static_cast<uint32_t>(validationLayers.size()))
				.setPpEnabledLayerNames(validationLayers.data());
		}
		else
		{
			createInfo.setEnabledLayerCount(0);
		}

		createInfo
			.setEnabledExtensionCount(static_cast<uint32_t>(extensions.size()))
			.setPpEnabledExtensionNames(extensions.data());

		m_Instance = vk::createInstance(createInfo);
		AX_ASSERT_CORE(m_Instance, "Failed to create Instance!");
	}

	void VKGraphicsContext::createDebugMessenger()
	{
		vk::DebugUtilsMessengerCreateInfoEXT debugInfo{};
		debugInfo
			.setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning)
			.setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance)
			.setPfnUserCallback(Utils::debugCallback)
			.setPUserData(nullptr);

		vk::Result result = Utils::createDebugUtilsMessenger(m_Instance, &debugInfo, nullptr, &m_Debug);
		AX_ASSERT_CORE(result == vk::Result::eSuccess, "Failed to create DebugMessengerEXT!");
	}

	void VKGraphicsContext::createSurfaceKHR(void* windowHandle)
	{
		VkSurfaceKHR rawSurface;
		if (glfwCreateWindowSurface(m_Instance, static_cast<GLFWwindow*>(windowHandle), nullptr, &rawSurface) != VK_SUCCESS)
		{
			AX_ASSERT_CORE(false, "Failed to create SurfaceKHR!");
		}

		m_Surface = rawSurface;
	}

	void VKGraphicsContext::createPhysicalDevice(const std::vector<const char*> deviceExtensions)
	{
		std::vector<vk::PhysicalDevice> devices = m_Instance.enumeratePhysicalDevices();
		AX_ASSERT_CORE(!devices.empty(), "Failed to find GPUs with Vulkan Support!");

		for (const auto& device : devices)
		{
			findQueueFamilies(device);

			if (Utils::isDeviceSuitable(device, m_Surface, deviceExtensions) && m_QueueFamilies.IsComplete())
			{
				m_PhysicalDevice = device;
				break;
			}
		}

		AX_ASSERT_CORE(m_PhysicalDevice, "Failed to find suitable PhysicalDevice!");
	}

	void VKGraphicsContext::findQueueFamilies(vk::PhysicalDevice device)
	{
		std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

		for (size_t i = 0; i < queueFamilies.size(); i++)
		{
			if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics && queueFamilies[i].queueFlags & vk::QueueFlagBits::eCompute)
			{
				m_QueueFamilies.GraphicsFamily = static_cast<uint32_t>(i);
			}

			VkBool32 presentSupport = device.getSurfaceSupportKHR(static_cast<uint32_t>(i), m_Surface);
			if (presentSupport)
			{
				m_QueueFamilies.PresentFamily = static_cast<uint32_t>(i);
			}

			if (m_QueueFamilies.IsComplete()) break;
		}
	}

	void VKGraphicsContext::createLogicalDevice(const std::vector<const char*> deviceExtensions, const std::vector<const char*> validationLayers)
	{
		std::unordered_set<uint32_t> uniqueQueues = { m_QueueFamilies.GraphicsFamily.value(), m_QueueFamilies.PresentFamily.value() };

		std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
		for (uint32_t queue : uniqueQueues)
		{
			vk::DeviceQueueCreateInfo queueCreateInfo{};
			float queuePriority = 1.0f;
			queueCreateInfo
				.setQueueFamilyIndex(queue)
				.setQueueCount(1)
				.setPQueuePriorities(&queuePriority);

			queueCreateInfos.push_back(queueCreateInfo);
		}

		vk::PhysicalDeviceFeatures deviceFeatures{};

		vk::DeviceCreateInfo createInfo{};
		createInfo
			.setPQueueCreateInfos(queueCreateInfos.data())
			.setQueueCreateInfoCount(static_cast<uint32_t>(queueCreateInfos.size()))
			.setPEnabledFeatures(&deviceFeatures)
			.setEnabledExtensionCount(static_cast<uint32_t>(deviceExtensions.size()))
			.setPpEnabledExtensionNames(deviceExtensions.data());

		if (!validationLayers.empty())
		{
			createInfo
				.setEnabledLayerCount(static_cast<uint32_t>(validationLayers.size()))
				.setPpEnabledLayerNames(validationLayers.data());
		}
		else
		{
			createInfo.setEnabledLayerCount(0);
		}

		m_Device = m_PhysicalDevice.createDevice(createInfo);
		AX_ASSERT_CORE(m_Device, "Failed to create LogicalDevice!");

		m_GraphicsQueue = m_Device.getQueue(m_QueueFamilies.GraphicsFamily.value(), 0);
		m_PresentQueue = m_Device.getQueue(m_QueueFamilies.PresentFamily.value(), 0);
		m_ComputeQueue = m_Device.getQueue(m_QueueFamilies.GraphicsFamily.value(), 0);
	}

	void VKGraphicsContext::createCommandPool()
	{
		vk::CommandPoolCreateInfo createInfo{};
		createInfo
			.setQueueFamilyIndex(m_QueueFamilies.GraphicsFamily.value())
			.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);

		m_CommandPool = m_Device.createCommandPool(createInfo);

		vk::CommandBufferAllocateInfo allocInfo{};
		allocInfo
			.setCommandPool(m_CommandPool)
			.setCommandBufferCount(VKRenderEngine::MAX_FRAMES_IN_FLIGHT)
			.setLevel(vk::CommandBufferLevel::ePrimary);

		m_CommandBuffers = m_Device.allocateCommandBuffers(allocInfo);
	}
}