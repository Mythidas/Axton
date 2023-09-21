#include "axpch.h"
#include "Application.h"

#include "Time.h"
#include "Input.h"
#include "Axton/Debug/Log.h"
#include "Axton/Event/Events.h"
#include "Axton/Renderer/Renderer2D.h"
#include "Axton/Renderer/RenderCommands.h"

namespace Axton
{
	Application::Application()
	{
		Log::Construct();
		CoreLog::Construct();
		Time::Construct();
		Input::Construct();

		m_Window = Window::Create();
		Renderer2D::Construct();

		CoreLog::Info("Application Created!");

		Events::OnWindowClose += AX_BIND_FNC(Application::OnWindowClose);
		Events::OnWindowResize += AX_BIND_FNC(Application::OnWindowResize);
	}

	void Application::Run()
	{
		while (m_Running)
		{
			Time::OnUpdate();
			m_Window->OnUpdate();

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			if (Time::GetFixedDeltaTime() > 0.0f)
			{
				for (Layer* layer : m_LayerStack)
				{
					layer->OnFixedUpdate();
				}
			}
		}
	}

	void Application::OnWindowClose()
	{
		m_Running = false;

		CoreLog::Info("Application Closing...");
	}

	void Application::OnWindowResize(int width, int height)
	{
		RenderCommands::SetViewport(width, height);
	}
}