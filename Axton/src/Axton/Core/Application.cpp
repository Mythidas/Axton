#include "axpch.h"
#include "Application.h"

#include "Time.h"
#include "Input.h"
#include "Axton/Debug/Log.h"
#include "Axton/Event/Events.h"
#include "Axton/ImGUI/ImGUILayer.h"

namespace Axton
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const Window::Specs& specs)
	{
		AX_ASSERT_CORE(!s_Instance, "Application already exists!");
		s_Instance = this;

		Log::Construct();
		CoreLog::Construct();
		Time::Construct();
		Input::Construct();

		m_Window = specs.Build();
		m_RenderEngine = RenderEngine::Create(m_Window->GetNativeWindow());

		m_ImGUILayer = ImGUILayer::Create();
		PushOverlay(m_ImGUILayer);

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

			//m_RenderEngine->BeginFrame();

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

			if (m_ImGUILayer)
			{
				m_ImGUILayer->BeginUI();
				for (Layer* layer : m_LayerStack)
				{
					layer->OnRenderUI();
				}
				m_ImGUILayer->EndUI();
			}

			//m_RenderEngine->EndFrame();
			m_RenderEngine->RenderFrame();
		}
	}

	void Application::OnWindowClose()
	{
		m_Running = false;

		CoreLog::Info("Application Closing...");
	}

	void Application::OnWindowResize(int width, int height)
	{
		// RendererAPI::SetViewport(width, height);
	}
}