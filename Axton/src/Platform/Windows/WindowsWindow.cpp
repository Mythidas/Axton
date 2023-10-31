#include "axpch.h"
#include "WindowsWindow.h"

#include "Axton/Core/Assert.h"
#include "Axton/Event/Events.h"
#include "Axton/Renderer/RendererAPI.h"

namespace Axton
{
	WindowsWindow::WindowsWindow(const Window::Specs& specs)
	{
		m_Specs = specs;

		int glfw = glfwInit();
		AX_ASSERT_CORE(glfw, "Failed to Init GLFW");


		if (RendererAPI::GetAPI() == RendererAPI::API::Vulkan)
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_Context = GraphicsContext::Create();
		m_Window = glfwCreateWindow((int)specs.Width, (int)specs.Height, specs.Title.c_str(), nullptr, nullptr);
		AX_ASSERT_CORE(m_Window, "Failed to create GLFW window");

		m_Context->Init(m_Window);

		glfwSwapInterval((int)specs.VSync);
		glfwSetWindowUserPointer(m_Window, this);

		if (specs.FixedAspectRatio)
			glfwSetWindowAspectRatio(m_Window, specs.Width, specs.Height);

		// Setup Callbacks GLFW

		glfwSetErrorCallback([](int error, const char* desc)
		{
			CoreLog::Error(desc);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			Events::OnWindowClose();
		});

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			Window::Specs& specs = ((WindowsWindow*)glfwGetWindowUserPointer(window))->m_Specs;
			specs.Width = width;
			specs.Height = height;

			Events::OnWindowResize(width, height);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (action == GLFW_PRESS)
				Events::OnKeyPressed(key);
			else if (action == GLFW_RELEASE)
				Events::OnKeyReleased(key);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
		{
			Events::OnMouseMoved(xpos, ypos);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			if (action == GLFW_PRESS)
			{
				Events::OnMouseButtonPressed(button);
			}
			else if (action == GLFW_RELEASE)
				Events::OnMouseButtonReleased(button);
		});

		CoreLog::Info("Window Created {0} {1}:{2}", specs.Title, specs.Width, specs.Height);
	}

	WindowsWindow::~WindowsWindow()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();

		CoreLog::Info("WindowsWindow Released");
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetCursorMode(bool locked)
	{
		if (locked)
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}