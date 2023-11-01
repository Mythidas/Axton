#include "axpch.h"
#include "OGLRendererAPI.h"

#include <glad/glad.h>

namespace Axton::OpenGL
{
	void OGLRendererAPI::clearScreen()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OGLRendererAPI::setClearColor(const Vector4& color)
	{
		glClearColor(color.x, color.y, color.z, color.w);
	}

	void OGLRendererAPI::setBlendMode(bool enabled)
	{
		if (enabled)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}

	void OGLRendererAPI::setViewport(int& width, int& height)
	{
		//glViewport(0, 0, width, height);
		if (int error = glGetError())
		{
			CoreLog::Error("OpenGL Viewport Error: {0}", error);
			AX_ASSERT_CORE(false, "OpenGL Error");
		}
	}

	void OGLRendererAPI::enableDepthTest(bool enabled)
	{
		if (enabled)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}

	void OGLRendererAPI::drawIndexed(uint32_t count)
	{
		if (int error = glGetError())
		{
			CoreLog::Critical("OpenGL:: Error: {0}", error);
		}
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
	}
}