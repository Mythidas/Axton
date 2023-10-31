#pragma once

#include "Axton/Renderer/RendererAPI.h"

#include <glad/glad.h>

namespace Axton::OpenGL
{
	class OGLUtils
	{
	public:
		static int ImageFormatSize(ImageFormat::Data format);
		static GLenum ImageFormatToGL(ImageFormat::Data format);
		static GLenum ImageFormatToGLInternal(ImageFormat::Data format);
		static GLenum WrapFormatToGL(ImageFormat::Wrap wrap);
		static GLenum FilterFormatToGL(ImageFormat::Filter filter);
		static GLenum AccessFormatToGL(ImageFormat::Access format);

		static void CheckForErrors(const std::string& info);
	};
}