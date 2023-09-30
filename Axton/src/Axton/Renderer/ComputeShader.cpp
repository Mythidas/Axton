#include "axpch.h"
#include "ComputeShader.h"
#include "Platform/OpenGL/OGLComputeShader.h"

namespace Axton
{
	Ref<ComputeShader> ComputeShader::Create(const std::string& srcPath)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGL::OGLComputeShader>(srcPath);
		}

		AX_ASSERT_CORE(false, "Unknown RendererAPI!");
		return nullptr;
	}
}