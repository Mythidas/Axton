#pragma once

#include "Axton/Renderer/GraphicsContext.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Axton::OpenGL
{
	class GraphicsContext : public Axton::GraphicsContext
	{
	public:
		GraphicsContext();

		virtual void Init(void* window) override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}