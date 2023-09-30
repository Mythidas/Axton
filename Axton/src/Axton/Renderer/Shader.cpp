#include "axpch.h"
#include "Shader.h"
#include "Axton/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OGLShader.h"

namespace Axton
{
	Ref<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc, bool lateLoad)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGL::OGLShader>(vertexSrc, fragmentSrc, lateLoad);
		}

		AX_ASSERT_CORE(false, "Unknown RendererAPI!");
		return nullptr;
	}
}