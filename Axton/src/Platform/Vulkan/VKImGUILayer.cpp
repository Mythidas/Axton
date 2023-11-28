#include "axpch.h"
#include "VKImGUILayer.h"
#include "VKRenderEngine.h"
#include "Axton/Core/Application.h"
#include "Client API/VKRenderPass.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

namespace Axton::Vulkan
{
	VKImGUILayer::VKImGUILayer()
	{
		vk::DescriptorPoolSize poolSizes[] =
		{
			vk::DescriptorPoolSize(vk::DescriptorType::eSampler, 1000),
			vk::DescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler, 1000),
			vk::DescriptorPoolSize(vk::DescriptorType::eSampledImage, 1000),
			vk::DescriptorPoolSize(vk::DescriptorType::eStorageImage, 1000),
			vk::DescriptorPoolSize(vk::DescriptorType::eUniformTexelBuffer, 1000),
			vk::DescriptorPoolSize(vk::DescriptorType::eStorageTexelBuffer, 1000),
			vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, 1000),
			vk::DescriptorPoolSize(vk::DescriptorType::eStorageBuffer, 1000),
			vk::DescriptorPoolSize(vk::DescriptorType::eUniformBufferDynamic, 1000),
			vk::DescriptorPoolSize(vk::DescriptorType::eStorageBufferDynamic, 1000),
			vk::DescriptorPoolSize(vk::DescriptorType::eInputAttachment, 1000)
		};

		vk::DescriptorPoolCreateInfo poolInfo{};
		poolInfo
			.setFlags(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet)
			.setMaxSets(1000)
			.setPoolSizeCount(static_cast<uint32_t>(std::size(poolSizes)))
			.setPPoolSizes(poolSizes);

		m_DescriptorPool = VKRenderEngine::GetDevice().createDescriptorPool(poolInfo);

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.DisplaySize = ImVec2(1280.0f, 720.0f);

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForVulkan(static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow()), true);

		Ref<VKGraphicsContext> gContext = VKRenderEngine::GetGraphicsContext();
		ImGui_ImplVulkan_InitInfo initInfo{};
		initInfo.Instance = gContext->GetInstance();
		initInfo.PhysicalDevice = gContext->GetPhysicalDevice();
		initInfo.Device = gContext->GetDevice();
		initInfo.Queue = gContext->GetGraphicsQueue();
		initInfo.DescriptorPool = m_DescriptorPool;
		initInfo.MinImageCount = VKRenderEngine::MAX_FRAMES_IN_FLIGHT;
		initInfo.ImageCount = VKRenderEngine::MAX_FRAMES_IN_FLIGHT;
		initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

		VKRenderPass& vkRenderPass = static_cast<VKRenderPass&>(*RenderEngine::GetRenderPass().get());
		ImGui_ImplVulkan_Init(&initInfo, vkRenderPass.operator vk::RenderPass());

		gContext->SubmitGraphicsCommand([&](vk::CommandBuffer buffer)
		{
			ImGui_ImplVulkan_CreateFontsTexture(buffer);
		});

		ImGui_ImplVulkan_DestroyFontUploadObjects();

		vk::Device device = gContext->GetDevice();
		gContext->QueueDeletion([device, this]()
		{
			device.destroy(m_DescriptorPool);
			ImGui_ImplVulkan_Shutdown();
		});
	}

	void VKImGUILayer::BeginUI() const
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	}

	void VKImGUILayer::EndUI() const
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)Application::Get().GetWindow()->GetWidth(), (float)Application::Get().GetWindow()->GetHeight());

		ImGui::Render();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backupContext = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backupContext);
		}

		VKRenderEngine::GetGraphicsContext()->QueueGraphicsCommand([this](vk::CommandBuffer& buffer)
		{
			ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), buffer);
		});
	}
}