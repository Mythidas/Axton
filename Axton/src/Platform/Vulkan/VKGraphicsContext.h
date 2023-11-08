#pragma once

#include <vulkan/vulkan.hpp>
#include <queue>

namespace Axton::Vulkan
{
	class VKGraphicsContext
	{
		struct QueueFamilies
		{
			std::optional<uint32_t> GraphicsFamily;
			std::optional<uint32_t> PresentFamily;

			bool IsComplete()
			{
				return GraphicsFamily.has_value() && PresentFamily.has_value();
			}
		};
	public:
		struct QueueSubmitInfo
		{
			std::vector<vk::Semaphore> WaitSemaphores;
			std::vector<vk::Semaphore> SignalSemaphores;
			std::vector<vk::PipelineStageFlags> WaitStages;
			vk::Fence Fence;
		};

	public:
		static Ref<VKGraphicsContext> Create(void* windowHandle, const std::vector<const char*> deviceExtensions, const std::vector<const char*> validationLayers);

		void Update();
		void QueueDeletion(std::function<void()> func);
		void QueueCommand(std::function<void(vk::CommandBuffer&)> func);
		void SubmitCommand(std::function<void(vk::CommandBuffer&)> func);
		void SubmitQueue(const QueueSubmitInfo& queueSubmitInfo);
		vk::Result PresentQueue(const QueueSubmitInfo& queueSubmitInfo, const std::vector<vk::SwapchainKHR>& swapchains, uint32_t imageIndex);

		void FlushCommandQueue();

		void Destroy();

		uint32_t GetCurrentFrame() { return m_CurrentFrame; }
		vk::Instance& GetInstance() { return m_Instance; }
		vk::SurfaceKHR& GetSurface() { return m_Surface; }
		vk::PhysicalDevice& GetPhysicalDevice() { return m_PhysicalDevice; }
		vk::Device& GetDevice() { return m_Device; }
		vk::Queue& GetGraphicsQueue() { return m_GraphicsQueue; }
		vk::Queue& GetPresentQueue() { return m_PresentQueue; }
		const std::vector<uint32_t> GetQueueFamilyIndices() { return { m_QueueFamilies.GraphicsFamily.value(), m_QueueFamilies.PresentFamily.value() }; }

	private:
		void createInstance(const std::vector<const char*> validationLayers);
		void createDebugMessenger();
		void createSurfaceKHR(void* windowHandle);
		void createPhysicalDevice(const std::vector<const char*> deviceExtensions);
		void findQueueFamilies(vk::PhysicalDevice device);
		void createLogicalDevice(const std::vector<const char*> deviceExtensions, const std::vector<const char*> validationLayers);
		void createCommandPool();

	private:
		vk::Instance m_Instance;
		vk::SurfaceKHR m_Surface;
		vk::DebugUtilsMessengerEXT m_Debug;
		uint32_t m_CurrentFrame = 0;
		std::queue<std::function<void()>> m_DeletionQueue;
		std::queue<std::function<void(vk::CommandBuffer&)>> m_CommandQueue;

		vk::PhysicalDevice m_PhysicalDevice;
		vk::Device m_Device;
		vk::Queue m_GraphicsQueue;
		vk::Queue m_PresentQueue;

		QueueFamilies m_QueueFamilies;

		vk::CommandPool m_CommandPool;
		std::vector<vk::CommandBuffer> m_CommandBuffers;
	};
}