#pragma once

#include "Axton/Renderer/RendererAPI.h"
#include "Axton/Renderer/Shader.h"
#include "Axton/Math/Math.h"

namespace Axton
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc, bool lateLoad);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void LoadShader() override;

		virtual void SetMat4(const std::string& name, const Matrix4& value) override;

	private:
		RendererID m_RendererID;
		bool m_LateLoad;

		std::string m_VertexSource;
		std::string m_FragmentSource;
	};
}