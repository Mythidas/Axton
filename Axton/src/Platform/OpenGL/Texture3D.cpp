#include "axpch.h"
#include "Texture3D.h"

#include <glad/glad.h>

namespace Axton
{
	namespace Utils
	{
		static GLenum TextureFormatToGL(ImageFormat format)
		{
			switch (format)
			{
				case ImageFormat::RGB8: return GL_RGB;
				case ImageFormat::RGBA8: return GL_RGBA;
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

		static void GLCreateTexture(RendererID& m_RendererID, int width, int height, int depth, const Texture3DSpecs& specs)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			glCreateTextures(GL_TEXTURE_3D, 1, &m_RendererID);
			glTextureStorage3D(m_RendererID, 1, Utils::TextureFormatToGLInternal(specs.Format), width, height, depth);

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, Utils::TextureFilterToGL(specs.Filter));
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, Utils::TextureFilterToGL(specs.Filter));

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, Utils::TextureWrapToGL(specs.Wrap));
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, Utils::TextureWrapToGL(specs.Wrap));
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, Utils::TextureWrapToGL(specs.Wrap));

			if (specs.GenerateMipmaps)
			{
				glBindTexture(GL_TEXTURE_3D, m_RendererID);
				glGenerateMipmap(GL_TEXTURE_3D);
				glBindTexture(GL_TEXTURE_3D, 0);
			}
		}
	}

	OpenGL::Texture3D::Texture3D(const Texture3DSpecs& specs)
		: m_Specs(specs)
	{
	}

	OpenGL::Texture3D::~Texture3D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGL::Texture3D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
		if (int error = glGetError())
		{
			CoreLog::Error("OpenGL Error: {0}", error);
			AX_ASSERT_CORE(false, "OpenGL Error Post Texture3D Bind!");
		}
	}

	void OpenGL::Texture3D::Unbind() const
	{
		glBindTexture(GL_TEXTURE_3D, 0);
	}

	void OpenGL::Texture3D::SetData(void* data, int size)
	{
		//uint32_t bpp = m_Specs.Format == ImageFormat::RGBA8 ? 4 : 3;
		//AX_ASSERT_CORE(size == m_Specs.Width * m_Specs.Height * m_Specs.Depth * bpp, "Data must be entire texture!");

		Utils::GLCreateTexture(m_RendererID, m_Specs.Width, m_Specs.Height, m_Specs.Depth, m_Specs);
		glTextureSubImage3D(m_RendererID, 0, 0, 0, 0, m_Specs.Width, m_Specs.Height, m_Specs.Depth, Utils::TextureFormatToGL(m_Specs.Format), GL_UNSIGNED_BYTE, data);
	}
}