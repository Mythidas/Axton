#include "axpch.h"
#include "OGLImage3D.h"
#include "OGLUtils.h"

#include <glad/glad.h>

namespace Axton::OpenGL
{
	OGLImage3D::OGLImage3D(const Image3DSpecs& specs)
		: m_RendererID(0), m_Specs(specs)
	{
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_3D, m_RendererID);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, OGLUtils::FilterFormatToGL(specs.Filter));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, OGLUtils::FilterFormatToGL(specs.Filter));

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, OGLUtils::WrapFormatToGL(specs.Wrap));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, OGLUtils::WrapFormatToGL(specs.Wrap));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, OGLUtils::WrapFormatToGL(specs.Wrap));

		glTexImage3D(GL_TEXTURE_3D, 0, OGLUtils::ImageFormatToGLInternal(specs.Format), (GLint)specs.Width, (GLint)specs.Height, (GLint)specs.Depth, 0,
			OGLUtils::ImageFormatToGL(specs.Format), GL_FLOAT, NULL);
		glBindImageTexture(specs.Slot, m_RendererID, 0, GL_FALSE, 0, OGLUtils::AccessFormatToGL(specs.Access), OGLUtils::ImageFormatToGLInternal(specs.Format));
	}

	OGLImage3D::~OGLImage3D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OGLImage3D::Bind() const
	{
		glBindImageTexture(m_Specs.Slot, m_RendererID, 0, GL_FALSE, 0, OGLUtils::AccessFormatToGL(m_Specs.Access), OGLUtils::ImageFormatToGLInternal(m_Specs.Format));
	}

	void OGLImage3D::SetData(void* data)
	{
		glTextureSubImage3D(m_RendererID, 0, 0, 0, 0, m_Specs.Width, m_Specs.Height, m_Specs.Depth, OGLUtils::ImageFormatToGL(m_Specs.Format), GL_UNSIGNED_BYTE, data);
	}

	void OGLImage3D::SetData(void* data, uint32_t width, uint32_t height, uint32_t depth)
	{
		OGLImage3D::Resize(width, height, depth);
		SetData(data);
	}

	void OGLImage3D::Resize(uint32_t width, uint32_t height, uint32_t depth)
	{
		if (width == m_Specs.Width && height == m_Specs.Height) return;

		m_Specs.Width = width;
		m_Specs.Height = height;

		glBindTexture(GL_TEXTURE_3D, m_RendererID);
		glTexImage3D(GL_TEXTURE_3D, 0, OGLUtils::ImageFormatToGLInternal(m_Specs.Format), (GLint)m_Specs.Width, (GLint)m_Specs.Height, (GLint)m_Specs.Depth, 0,
			OGLUtils::ImageFormatToGL(m_Specs.Format), GL_FLOAT, NULL);
	}
}