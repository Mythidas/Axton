#include "axpch.h"
#include "RenderCommands.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLRenderCommands.h"

namespace Axton
{
	void RenderCommands::ClearScreen()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return;
			case RendererAPI::API::OpenGL:  OpenGLRenderCommands::Clear(); return;
		}
	}

	void RenderCommands::SetClearColor(const Vector4& clearColor)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return;
			case RendererAPI::API::OpenGL:  OpenGLRenderCommands::SetClearColor(clearColor); return;
		}
	}

	void RenderCommands::SetBlendMode(bool enabled)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return;
			case RendererAPI::API::OpenGL:  OpenGLRenderCommands::SetBlendMode(enabled); return;
		}
	}

	void RenderCommands::SetViewport(int& width, int& height)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return;
			case RendererAPI::API::OpenGL:  OpenGLRenderCommands::SetViewport(width, height); return;
		}
	}

	void RenderCommands::DrawIndexed(uint32_t count)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); return;
			case RendererAPI::API::OpenGL:  OpenGLRenderCommands::DrawIndexed(count); return;
		}
	}
}