#include "axpch.h"
#include "Application.h"

#include "Time.h"
#include "Axton/Debug/Log.h"
#include "Axton/Event/Events.h"
#include "Axton/Renderer/Renderer2D.h"

namespace Axton
{
	Application::Application()
	{
		Log::Construct();
		CoreLog::Construct();
		Time::Construct();

		m_Window = Window::Create();
		Renderer2D::Construct();

		CoreLog::Info("Application Created!");

		Events::OnWindowClose += AX_BIND_FNC(Application::OnWindowClose);
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
}