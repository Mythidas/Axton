#include "axpch.h"
#include "Texture2D.h"

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

		static void GLCreateTexture(RendererID& m_RendererID, uint32_t width, uint32_t height, const Texture2DSpecs& specs)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, Utils::TextureFormatToGLInternal(specs.Format), width, height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, Utils::TextureFilterToGL(specs.Filter));
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, Utils::TextureFilterToGL(specs.Filter));

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, Utils::TextureWrapToGL(specs.Wrap));
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, Utils::TextureWrapToGL(specs.Wrap));

			if (specs.GenerateMipmaps)
			{
				glBindTexture(GL_TEXTURE_2D, m_RendererID);
				glGenerateMipmap(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
	}

	OpenGL::Texture2D::Texture2D(const Texture2DSpecs& specs)
		: m_Specs(specs)
	{
		Utils::GLCreateTexture(m_RendererID, m_Specs.Width, m_Specs.Height, m_Specs);
	}

	OpenGL::Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGL::Texture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
		AX_ASSERT_CORE(glGetError() == 0, "OpenGL Error Post Bind!");
	}

	void OpenGL::Texture2D::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGL::Texture2D::SetData(void* data, uint32_t size)
	{
		size_t bpp = m_Specs.Format == ImageFormat::RGBA8 ? 4 : 3;
		AX_ASSERT_CORE(size == m_Specs.Width * m_Specs.Height * bpp, "Texture2D Data size must fill entire Texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Specs.Width, m_Specs.Height, Utils::TextureFormatToGL(m_Specs.Format), GL_UNSIGNED_BYTE, data);
	}

	void OpenGL::Texture2D::SetData(const std::string& path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
			AX_ASSERT_CORE(data, "Failed to load image!");
		}

		m_Specs.Width = width; m_Specs.Height = height;
		Utils::GLCreateTexture(m_RendererID, m_Specs.Width, m_Specs.Height, m_Specs);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Specs.Width, m_Specs.Height, Utils::TextureFormatToGL(m_Specs.Format), GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}
}