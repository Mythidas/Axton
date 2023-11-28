#include "axpch.h"
#include "WindowsWindow.h"

#include "Axton/Renderer/RenderEngine.h"

namespace Axton
{
	WindowsWindow::WindowsWindow(const Window::Specs& specs)
	{
		m_Specs = specs;

		int glfw = glfwInit();
		AssertCore(glfw, "Failed to Init GLFW");


		if (RenderEngine::GetAPI() == RenderAPI::Vulkan)
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_Window = glfwCreateWindow(specs.Width, specs.Height, specs.Title.c_str(), nullptr, nullptr);
		AssertCore(m_Window, "Failed to create GLFW window");

		glfwMakeContextCurrent(m_Window);

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
			Window::OnWindowClose();
		});

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowsWindow* wWindow = ((WindowsWindow*)glfwGetWindowUserPointer(window));
			wWindow->m_Specs.Width = width;
			wWindow->m_Specs.Height = height;

			Window::OnWindowResize(width, height);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (action == GLFW_PRESS)
				Window::OnKeyPressed(key);
			else if (action == GLFW_RELEASE)
				Window::OnKeyReleased(key);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
		{
				Window::OnMouseMoved(xpos, ypos);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			if (action == GLFW_PRESS)
			{
				Window::OnMouseButtonPressed(button);
			}
			else if (action == GLFW_RELEASE)
				Window::OnMouseButtonReleased(button);
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
	}

	void WindowsWindow::SetCursorMode(bool locked)
	{
		if (locked)
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}