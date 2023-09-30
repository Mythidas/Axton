#pragma once

#include "Axton/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Axton::OpenGL
{
	class OGLGraphicsContext : public Axton::GraphicsContext
	{
	public:
		OGLGraphicsContext();

		virtual void Init(void* window) override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}