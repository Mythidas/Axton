#include "axpch.h"
#include "OGLGraphicsContext.h"
#include "Axton/Core/Assert.h"
#include "Axton/Renderer/RenderCommands.h"

#include <glad/glad.h>

namespace Axton::OpenGL
{
	OGLGraphicsContext::OGLGraphicsContext()
		: m_WindowHandle(nullptr)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	}

	void OGLGraphicsContext::Init(void* windowHandle)
	{
		m_WindowHandle = static_cast<GLFWwindow*>(windowHandle);
		AX_ASSERT_CORE(m_WindowHandle, "Window handle is null!");

		glfwMakeContextCurrent(m_WindowHandle);
		int glad = gladLoadGL();
		AX_ASSERT_CORE(glad, "Failed to load GLAD GL!");
	}

	void OGLGraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
		RenderCommands::SetClearColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		RenderCommands::ClearScreen();
	}
}