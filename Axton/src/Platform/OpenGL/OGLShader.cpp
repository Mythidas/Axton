#include "axpch.h"
#include "OGLShader.h"
#include "Axton/Utils/FileSystem.h"

#include <glad/glad.h>

namespace Axton::OpenGL
{
	namespace Utils
	{
		static void LogVertexShaderErrors(RendererID vertexShader)
		{
			int success;
			char infoLog[512];

			// Check for errors
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				CoreLog::Error("Vertex Shader Compilation Failed: {0}", infoLog);
			}
		}

		static void LogFragmentShaderErrors(RendererID fragmentShader)
		{
			int success;
			char infoLog[512];

			// Check for errors
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
				CoreLog::Error("Fragment Shader Compilation Failed: {0}", infoLog);
			}
		}

		static void LogShaderProgramErrors(RendererID program)
		{
			int success;
			char infoLog[512];

			// Check for errors
			glGetProgramiv(program, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(program, 512, NULL, infoLog);
				CoreLog::Error("Shader Program Linking Failed: {0}", infoLog);
			}
		}
	}

	OGLShader::OGLShader(const std::string& vertexSrc, const std::string& fragmentSrc, bool lateLoad)
		: m_VertexSource(vertexSrc), m_FragmentSource(fragmentSrc), m_LateLoad(lateLoad)
	{
		if (!lateLoad) LoadShader();
	}

	OGLShader::~OGLShader()
	{
		glDeleteShader(m_RendererID);
	}

	void OGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OGLShader::LoadShader()
	{
		std::string vertFile = FileSystem(m_VertexSource).ToString();
		std::string fragFile = FileSystem(m_FragmentSource).ToString();

		const char* vertexSource = vertFile.c_str();
		const char* fragmentSource = fragFile.c_str();

		// Vertex Shader

		uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);

		Utils::LogVertexShaderErrors(vertexShader);

		// Fragment Shader

		uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);

		Utils::LogFragmentShaderErrors(fragmentShader);

		// Shader Program

		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);
		glLinkProgram(m_RendererID);

		Utils::LogShaderProgramErrors(m_RendererID);

		// Cleanup Shaders

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void OGLShader::SetMat4(const std::string& name, const Matrix4& value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
	}

	void OGLShader::SetInt(const std::string& name, const int value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OGLShader::SetIntArray(const std::string& name, const int value[], size_t size)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, (GLsizei)size, value);
	}
}