#include "axpch.h"
#include "OGLComputeShader.h"
#include "OGLUtils.h"
#include "Axton/Utils/FileSystem.h"

#include <glad/glad.h>

namespace Axton::OpenGL
{
	namespace Utils
	{
		static void LogComputeShaderErrors(RendererID computeShader)
		{
			int success;
			char infoLog[512];

			// Check for errors
			glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(computeShader, 512, NULL, infoLog);
				CoreLog::Error("Vertex Shader Compilation Failed: {0}", infoLog);
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

	OGLComputeShader::OGLComputeShader(const std::string& srcPath)
		: m_RendererID(0)
	{
		FileSystem srcFile(srcPath);
		std::string srcCode = srcFile.ToString();
		const GLchar* srcData[] = { srcCode.c_str() };

		RendererID computeID;
		computeID = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource(computeID, 1, srcData, NULL);
		glCompileShader(computeID);
		Utils::LogComputeShaderErrors(computeID);

		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, computeID);
		glLinkProgram(m_RendererID);
		Utils::LogShaderProgramErrors(m_RendererID);

		OGLUtils::CheckForErrors("Compute Shader");
	}

	OGLComputeShader::~OGLComputeShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OGLComputeShader::Dispatch(uint32_t width, uint32_t height, uint32_t depth) const
	{
		glUseProgram(m_RendererID);
		glDispatchCompute(width, height, depth);
	}

	void OGLComputeShader::Barrier() const
	{
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	}
}