#include "axpch.h"
#include "OGLImage1D.h"
#include "OGLUtils.h"

#include <glad/glad.h>

namespace Axton::OpenGL
{
	OGLImage1D::OGLImage1D(const Image1DSpecs& specs)
		: m_RendererID(0), m_Specs(specs)
	{
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_1D, m_RendererID);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, OGLUtils::FilterFormatToGL(specs.Filter));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, OGLUtils::FilterFormatToGL(specs.Filter));

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, OGLUtils::WrapFormatToGL(specs.Wrap));

		glTexImage1D(GL_TEXTURE_1D, 0, OGLUtils::ImageFormatToGLInternal(specs.Format), (GLint)specs.Width, 0,
			OGLUtils::ImageFormatToGL(specs.Format), GL_UNSIGNED_BYTE, NULL);
		glBindImageTexture(specs.Slot, m_RendererID, 0, GL_FALSE, 0, OGLUtils::AccessFormatToGL(specs.Access), OGLUtils::ImageFormatToGLInternal(specs.Format));

		OGLUtils::CheckForErrors("Image");
	}

	OGLImage1D::~OGLImage1D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OGLImage1D::Bind() const
	{
		glBindImageTexture(m_Specs.Slot, m_RendererID, 0, GL_UNSIGNED_BYTE, 0, OGLUtils::AccessFormatToGL(m_Specs.Access), OGLUtils::ImageFormatToGLInternal(m_Specs.Format));
	}

	void OGLImage1D::SetData(void* data, uint32_t width)
	{
		OGLImage1D::Resize(width);
		glTextureSubImage1D(m_RendererID, 0, 0, m_Specs.Width, OGLUtils::ImageFormatToGL(m_Specs.Format), GL_UNSIGNED_BYTE, data);
	}

	void OGLImage1D::Resize(uint32_t width)
	{
		if (width == m_Specs.Width) return;

		m_Specs.Width = width;

		glBindTexture(GL_TEXTURE_1D, m_RendererID);
		glTexImage1D(GL_TEXTURE_1D, 0, OGLUtils::ImageFormatToGLInternal(m_Specs.Format), (GLint)m_Specs.Width, 0,
			OGLUtils::ImageFormatToGL(m_Specs.Format), GL_FLOAT, NULL);
	}
}