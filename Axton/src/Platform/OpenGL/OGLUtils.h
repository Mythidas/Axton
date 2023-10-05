#pragma once

#include "Axton/Renderer/RendererAPI.h"

#include <glad/glad.h>

namespace Axton::OpenGL
{
	class OGLUtils
	{
	public:
		static GLenum ImageFormatToGL(ImageFormat format);
		static GLenum ImageFormatToGLInternal(ImageFormat format);
		static GLenum WrapFormatToGL(WrapFormat wrap);
		static GLenum FilterFormatToGL(FilterFormat filter);
		static GLenum AccessFormatToGL(AccessFormat format);
	};
}