#include "axpch.h"
#include "OGLImage.h"

#include <glad/glad.h>

namespace Axton::OpenGL
{
	namespace Utils
	{
		static GLenum TextureFormatToGL(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::RGB8: return GL_RGB;
			case ImageFormat::RGBA8: return GL_RGBA;
			case ImageFormat::RGBA32F: return GL_RGBA;
			}

			AX_ASSERT_CORE(false, "Unknown TextureFormat!");
			return 0;
		}

		static GLenum TextureFormatToGLInternal(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::RGB8: return GL_RGB8;
			case ImageFormat::RGBA8: return GL_RGBA8;
			case ImageFormat::RGBA32F: return GL_RGBA32F;
			}

			AX_ASSERT_CORE(false, "Unknown TextureFormat!");
			return 0;
		}

		static GLenum TextureWrapToGL(WrapFormat wrap)
		{
			switch (wrap)
			{
			case WrapFormat::REPEAT: return GL_REPEAT;
			case WrapFormat::CLAMP: return GL_CLAMP_TO_EDGE;
			}

			AX_ASSERT_CORE(false, "Unknown TextureWrap!");
			return 0;
		}

		static GLenum TextureFilterToGL(FilterFormat filter)
		{
			switch (filter)
			{
			case FilterFormat::NEAREST: return GL_NEAREST;
			case FilterFormat::LINEAR: return GL_LINEAR;
			}

			AX_ASSERT_CORE(false, "Unknown TextureFilter!");
			return 0;
		}

		static void CreateTextureImage(GLuint& rendererID, const ImageSpecs& specs)
		{
			glGenTextures(1, &rendererID);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, rendererID);

			glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, Utils::TextureFilterToGL(specs.Filter));
			glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, Utils::TextureFilterToGL(specs.Filter));

			glTextureParameteri(rendererID, GL_TEXTURE_WRAP_S, Utils::TextureWrapToGL(specs.Wrap));
			glTextureParameteri(rendererID, GL_TEXTURE_WRAP_T, Utils::TextureWrapToGL(specs.Wrap));

			glTexImage2D(GL_TEXTURE_2D, 0, Utils::TextureFormatToGLInternal(specs.Format), (GLint)specs.Width, (GLint)specs.Height, 0,
				Utils::TextureFormatToGL(specs.Format), GL_FLOAT, NULL);
			glBindImageTexture(0, rendererID, 0, GL_FALSE, 0, GL_WRITE_ONLY, Utils::TextureFormatToGLInternal(specs.Format));
		}
	}

	OGLImage::OGLImage(const ImageSpecs& specs)
		: m_RendererID(0), m_Specs(specs)
	{
		Utils::CreateTextureImage(m_RendererID, specs);
	}

	OGLImage::~OGLImage()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OGLImage::Bind() const
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void OGLImage::SetData(void* data, uint32_t width, uint32_t height)
	{
		OGLImage::Resize(width, height);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Specs.Width, m_Specs.Height, Utils::TextureFormatToGL(m_Specs.Format), GL_UNSIGNED_BYTE, data);
	}

	void OGLImage::Resize(uint32_t width, uint32_t height)
	{
		if (width == m_Specs.Width && height == m_Specs.Height) return;

		m_Specs.Width = width;
		m_Specs.Height = height;

		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, Utils::TextureFormatToGLInternal(m_Specs.Format), (GLint)m_Specs.Width, (GLint)m_Specs.Height, 0,
			Utils::TextureFormatToGL(m_Specs.Format), GL_FLOAT, NULL);
	}
}