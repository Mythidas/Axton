#include "axpch.h"
#include "OGLUtils.h"

namespace Axton::OpenGL
{
	int OGLUtils::ImageFormatSize(ImageFormat::Data format)
	{
		switch (format)
		{
		case ImageFormat::Data::R8: return 1;
		case ImageFormat::Data::RGB8: return 3;
		case ImageFormat::Data::RGBA8: return 4;
		case ImageFormat::Data::RGBA32F: return 4 * 4;
		}

		AX_ASSERT_CORE(false, "Unknown TextureFormat!");
		return 0;
	}

	GLenum OGLUtils::ImageFormatToGL(ImageFormat::Data format)
	{
		switch (format)
		{
		case ImageFormat::Data::R8: return GL_RED;
		case ImageFormat::Data::RGB8: return GL_RGB;
		case ImageFormat::Data::RGBA8: return GL_RGBA;
		case ImageFormat::Data::RGBA32F: return GL_RGBA;
		}

		AX_ASSERT_CORE(false, "Unknown TextureFormat!");
		return 0;
	}

	GLenum OGLUtils::ImageFormatToGLInternal(ImageFormat::Data format)
	{
		switch (format)
		{
		case ImageFormat::Data::R8: return GL_R8;
		case ImageFormat::Data::RGB8: return GL_RGB8;
		case ImageFormat::Data::RGBA8: return GL_RGBA8;
		case ImageFormat::Data::RGBA32F: return GL_RGBA32F;
		}

		AX_ASSERT_CORE(false, "Unknown TextureFormat!");
		return 0;
	}

	GLenum OGLUtils::WrapFormatToGL(ImageFormat::Wrap wrap)
	{
		switch (wrap)
		{
		case ImageFormat::Wrap::REPEAT: return GL_REPEAT;
		case ImageFormat::Wrap::CLAMP: return GL_CLAMP_TO_EDGE;
		}

		AX_ASSERT_CORE(false, "Unknown TextureWrap!");
		return 0;
	}

	GLenum OGLUtils::FilterFormatToGL(ImageFormat::Filter filter)
	{
		switch (filter)
		{
		case ImageFormat::Filter::NEAREST: return GL_NEAREST;
		case ImageFormat::Filter::LINEAR: return GL_LINEAR;
		}

		AX_ASSERT_CORE(false, "Unknown TextureFilter!");
		return 0;
	}

	GLenum OGLUtils::AccessFormatToGL(ImageFormat::Access format)
	{
		switch (format)
		{
		case ImageFormat::Access::READ_WRITE: return GL_READ_WRITE;
		case ImageFormat::Access::READ_ONLY: return GL_READ_ONLY;
		case ImageFormat::Access::WRITE_ONLY: return GL_WRITE_ONLY;
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