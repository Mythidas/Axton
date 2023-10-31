#include "axpch.h"
#include "OGLTexture3D.h"
#include "OGLUtils.h"

#include <glad/glad.h>

namespace Axton::OpenGL
{
	OGLTexture3D::OGLTexture3D(const Texture3DSpecs& specs)
		: m_Specs(specs)
	{
		glCreateTextures(GL_TEXTURE_3D, 1, &m_RendererID);
		glTextureStorage3D(m_RendererID, 1, OGLUtils::ImageFormatToGLInternal(specs.Format), specs.Width, specs.Height, specs.Depth);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, OGLUtils::FilterFormatToGL(specs.Filter));
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, OGLUtils::FilterFormatToGL(specs.Filter));

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, OGLUtils::WrapFormatToGL(specs.Wrap));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, OGLUtils::WrapFormatToGL(specs.Wrap));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, OGLUtils::WrapFormatToGL(specs.Wrap));

		if (specs.GenerateMipmaps)
		{
			glBindTexture(GL_TEXTURE_3D, m_RendererID);
			glGenerateMipmap(GL_TEXTURE_3D);
			glBindTexture(GL_TEXTURE_3D, 0);
		}

		OGLUtils::CheckForErrors("Texture3D");
	}

	OGLTexture3D::~OGLTexture3D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OGLTexture3D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	void OGLTexture3D::Unbind() const
	{
		glBindTexture(GL_TEXTURE_3D, 0);
	}

	void OGLTexture3D::SetData(void* data, IVector3 offset)
	{
		glTextureSubImage3D(m_RendererID, 0, offset.x, offset.y, offset.z, m_Specs.Width, m_Specs.Height, m_Specs.Depth, OGLUtils::ImageFormatToGL(m_Specs.Format), GL_UNSIGNED_BYTE, data);
	}
}