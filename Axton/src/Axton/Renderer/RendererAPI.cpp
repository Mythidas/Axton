#include "axpch.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OGLRendererAPI.h"
#include "Platform/Vulkan/VKRendererAPI.h"

namespace Axton
{
	RendererAPI::API RendererAPI::s_API = API::OpenGL;
	Scope<RendererAPI> RendererAPI::s_Instance = nullptr;

	void RendererAPI::Construct()
	{
		if (s_Instance)
		{
			AX_ASSERT_CORE(false, "RendererAPI already constructed!");
			return;
		}

		switch (s_API)
		{
		case RendererAPI::API::None:    AX_ASSERT_CORE(false, "RendererAPI::None is currently not supported!"); break;
		case RendererAPI::API::OpenGL:  s_Instance = CreateScope<OpenGL::OGLRendererAPI>(); break;
		case RendererAPI::API::Vulkan:  s_Instance = CreateScope<Vulkan::VKRendererAPI>(); break;
		}
	}

	void RendererAPI::ClearScreen()
	{
		s_Instance->clearScreen();
	}

	void RendererAPI::SetClearColor(const Vector4& clearColor)
	{
		s_Instance->setClearColor(clearColor);
	}

	void RendererAPI::SetBlendMode(bool enabled)
	{
		s_Instance->setBlendMode(enabled);
	}

	void RendererAPI::SetViewport(int& width, int& height)
	{
		s_Instance->setViewport(width, height);
	}

	void RendererAPI::EnableDepthTest(bool enabled)
	{
		s_Instance->enableDepthTest(enabled);
	}

	void RendererAPI::DrawIndexed(uint32_t count)
	{
		s_Instance->drawIndexed(count);
	}
}