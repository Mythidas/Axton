#pragma once

#include "Axton/Renderer/RendererAPI.h"
#include "Axton/Renderer/Shader.h"
#include "Axton/Math/Math.h"

namespace Axton::OpenGL
{
	class OGLShader : public Axton::Shader
	{
	public:
		OGLShader(const std::string& vertexSrc, const std::string& fragmentSrc, bool lateLoad);
		~OGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void LoadShader() override;

		virtual void SetMat4(const std::string& name, const Matrix4& value) override;
		virtual void SetInt(const std::string& name, const int value) override;
		virtual void SetIntArray(const std::string& name, const int value[], size_t size) override;

	private:
		RendererID m_RendererID = 0;
		bool m_LateLoad = false;

		std::string m_VertexSource;
		std::string m_FragmentSource;
	};
}