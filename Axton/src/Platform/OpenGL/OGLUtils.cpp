#include "axpch.h"
#include "OGLUtils.h"

namespace Axton::OpenGL
{
	int OGLUtils::ImageFormatSize(ImageFormat format)
	{
		switch (format)
		{
		case ImageFormat::R8: return 1;
		case ImageFormat::RGB8: return 3;
		case ImageFormat::RGBA8: return 4;
		case ImageFormat::RGBA32F: return 4 * 4;
		}

		AX_ASSERT_CORE(false, "Unknown TextureFormat!");
		return 0;
	}

	GLenum OGLUtils::ImageFormatToGL(ImageFormat format)
	{
		switch (format)
		{
		case ImageFormat::R8: return GL_RED;
		case ImageFormat::RGB8: return GL_RGB;
		case ImageFormat::RGBA8: return GL_RGBA;
		case ImageFormat::RGBA32F: return GL_RGBA;
		}

		AX_ASSERT_CORE(false, "Unknown TextureFormat!");
		return 0;
	}

	GLenum OGLUtils::ImageFormatToGLInternal(ImageFormat format)
	{
		switch (format)
		{
		case ImageFormat::R8: return GL_R8;
		case ImageFormat::RGB8: return GL_RGB8;
		case ImageFormat::RGBA8: return GL_RGBA8;
		case ImageFormat::RGBA32F: return GL_RGBA32F;
		}

		AX_ASSERT_CORE(false, "Unknown TextureFormat!");
		return 0;
	}

	GLenum OGLUtils::WrapFormatToGL(WrapFormat wrap)
	{
		switch (wrap)
		{
		case WrapFormat::REPEAT: return GL_REPEAT;
		case WrapFormat::CLAMP: return GL_CLAMP_TO_EDGE;
		}

		AX_ASSERT_CORE(false, "Unknown TextureWrap!");
		return 0;
	}

	GLenum OGLUtils::FilterFormatToGL(FilterFormat filter)
	{
		switch (filter)
		{
		case FilterFormat::NEAREST: return GL_NEAREST;
		case FilterFormat::LINEAR: return GL_LINEAR;
		}

		AX_ASSERT_CORE(false, "Unknown TextureFilter!");
		return 0;
	}

	GLenum OGLUtils::AccessFormatToGL(AccessFormat format)
	{
		switch (format)
		{
		case AccessFormat::READ_WRITE: return GL_READ_WRITE;
		case AccessFormat::READ_ONLY: return GL_READ_ONLY;
		case AccessFormat::WRITE_ONLY: return GL_WRITE_ONLY;
		}

		AX_ASSERT_CORE(false, "Unknown TextureFilter!");
		return 0;
	}

	void OGLUtils::CheckForErrors(const std::string& info)
	{
		if (int error = glGetError())
		{
			CoreLog::Error("OpenGL {0} Error: {1}", info, error);
			AX_ASSERT_CORE(false, "OpenGL Error");
		}
	}
}