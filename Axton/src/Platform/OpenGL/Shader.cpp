#include "axpch.h"
#include "Shader.h"
#include "Axton/Utils/FileSystem.h"

#include <glad/glad.h>

namespace Axton
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

	OpenGL::Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc, bool lateLoad)
		: m_VertexSource(vertexSrc), m_FragmentSource(fragmentSrc), m_LateLoad(lateLoad)
	{
		if (!lateLoad) LoadShader();
	}

	OpenGL::Shader::~Shader()
	{
		glDeleteShader(m_RendererID);
	}

	void OpenGL::Shader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGL::Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGL::Shader::LoadShader()
	{
		std::string vertFile = FileSystem(m_VertexSource).ReadFile();
		std::string fragFile = FileSystem(m_FragmentSource).ReadFile();

		const char* vertexSource = vertFile.c_str();
		const char* fragmentSource = fragFile.c_str();

		int success;
		char infoLog[512];

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

	void OpenGL::Shader::SetMat4(const std::string& name, const Matrix4& value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
	}

	void OpenGL::Shader::SetInt(const std::string& name, const int value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGL::Shader::SetIntArray(const std::string& name, const int value[], size_t size)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, size, value);
	}
}