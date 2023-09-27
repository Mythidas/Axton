#include "axpch.h"
#include "RendererAPI.h"

#include <glad/glad.h>

namespace Axton
{
	void OpenGL::RendererAPI::ClearScreen()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGL::RendererAPI::SetClearColor(const Vector4& color)
	{
		glClearColor(color.x, color.y, color.z, color.w);
	}

	void OpenGL::RendererAPI::SetBlendMode(bool enabled)
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

	void OpenGL::RendererAPI::SetViewport(int& width, int& height)
	{
		glViewport(0, 0, width, height);
		AX_ASSERT_CORE(glGetError() == GL_NO_ERROR, "OpenGL:: Error!");
	}

	void OpenGL::RendererAPI::EnableDepthTest(bool enabled)
	{
		if (enabled)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}

	void OpenGL::RendererAPI::DrawIndexed(uint32_t count)
	{
		if (int error = glGetError())
		{
			CoreLog::Critical("OpenGL:: Error: {0}", error);
		}
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
	}
}