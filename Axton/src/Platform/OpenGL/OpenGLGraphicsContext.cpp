#include "axpch.h"
#include "OpenGLGraphicsContext.h"
#include "Axton/Core/Assert.h"
#include "Axton/Renderer/RenderCommands.h"

#include <glad/glad.h>

namespace Axton
{
	OpenGLGraphicsContext::OpenGLGraphicsContext(GLFWwindow* window)
		: m_WindowHandle(window)
	{
		AX_ASSERT_CORE(m_WindowHandle, "Window handle is null!");
	}

	void OpenGLGraphicsContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		AX_ASSERT_CORE(gladLoadGL(), "Failed to load GLAD GL!");
	}

	void OpenGLGraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}