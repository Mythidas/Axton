#include "axpch.h"
#include "OpenGLRenderCommands.h"

#include <glad/glad.h>

namespace Axton
{
	void OpenGLRenderCommands::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRenderCommands::SetClearColor(const Vector4& color)
	{
		glClearColor(color.x, color.y, color.z, color.w);
	}

	void OpenGLRenderCommands::SetBlendMode(bool enabled)
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

	void OpenGLRenderCommands::DrawIndexed(uint32_t count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
	}
}