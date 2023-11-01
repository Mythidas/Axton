#include "axpch.h"
#include "OGLGraphicsContext.h"
#include "Axton/Core/Assert.h"
#include "Axton/Renderer/Image3D.h"

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

		// TODO: Add requirements for GPU like Vulkan
		GLint maxImageUnits;
		glGetIntegerv(GL_MAX_IMAGE_UNITS, &maxImageUnits);
		CoreLog::Info("OpenGL: Max Image Units: {0}", maxImageUnits);

		GLint maxTexture1DSize;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexture1DSize);
		CoreLog::Info("OpenGL: Max Texture1D Size: {0}", maxTexture1DSize);

		GLint maxTexture3DSize;
		glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &maxTexture3DSize);
		CoreLog::Info("OpenGL: Max Texture3D Size: {0}", maxTexture3DSize);
	}

	void OGLGraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
		RendererAPI::SetClearColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		RendererAPI::ClearScreen();
	}
}