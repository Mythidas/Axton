#include "axpch.h"
#include "WindowsWindow.h"
#include "Axton/Core/Assert.h"
#include "Axton/Event/Events.h"

namespace Axton
{
	WindowsWindow::WindowsWindow(const WindowSpecs& specs)
	{
		m_Specs = specs;

		AX_ASSERT_CORE(glfwInit(), "Failed to Init GLFW");

		m_Window = glfwCreateWindow((int)specs.Width, (int)specs.Height, specs.Title.c_str(), nullptr, nullptr);
		AX_ASSERT_CORE(m_Window, "Failed to create GLFW window");

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

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
			WindowSpecs& specs = ((WindowsWindow*)glfwGetWindowUserPointer(window))->m_Specs;
			specs.Width = width;
			specs.Height = height;

			Events::OnWindowResize(width, height);
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
}