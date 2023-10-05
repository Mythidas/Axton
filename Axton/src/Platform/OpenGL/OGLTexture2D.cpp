#include "axpch.h"
#include "OGLTexture2D.h"
#include "OGLUtils.h"

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Axton::OpenGL
{
	namespace Utils
	{
		static void GLCreateTexture(RendererID& m_RendererID, uint32_t width, uint32_t height, const Texture2DSpecs& specs)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, OGLUtils::ImageFormatToGLInternal(specs.Format), width, height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, OGLUtils::FilterFormatToGL(specs.Filter));
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, OGLUtils::FilterFormatToGL(specs.Filter));

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, OGLUtils::WrapFormatToGL(specs.Wrap));
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, OGLUtils::WrapFormatToGL(specs.Wrap));

			if (specs.GenerateMipmaps)
			{
				glBindTexture(GL_TEXTURE_2D, m_RendererID);
				glGenerateMipmap(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
	}

	OGLTexture2D::OGLTexture2D(const Texture2DSpecs& specs)
		: m_Specs(specs)
	{
		Utils::GLCreateTexture(m_RendererID, m_Specs.Width, m_Specs.Height, m_Specs);
	}

	OGLTexture2D::~OGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
		AX_ASSERT_CORE(glGetError() == 0, "OpenGL Error Post Bind!");
	}

	void OGLTexture2D::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OGLTexture2D::SetData(void* data, uint32_t size)
	{
		size_t bpp = m_Specs.Format == ImageFormat::RGBA8 ? 4 : 3;
		AX_ASSERT_CORE(size == m_Specs.Width * m_Specs.Height * bpp, "Texture2D Data size must fill entire Texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Specs.Width, m_Specs.Height, OGLUtils::ImageFormatToGL(m_Specs.Format), GL_UNSIGNED_BYTE, data);
	}

	void OGLTexture2D::SetData(const std::string& path)
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
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Specs.Width, m_Specs.Height, OGLUtils::ImageFormatToGL(m_Specs.Format), GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}
}