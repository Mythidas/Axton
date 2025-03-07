#include "axpch.h"
#include "Application.h"

#include "Time.h"
#include "Input.h"
#include "Axton/Debug/Log.h"
#include "Axton/ImGUI/ImGUILayer.h"

namespace Axton
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const Specs& specs)
	{
		AssertCore(!s_Instance, "Application already exists!");
		s_Instance = this;

		Log::Construct();
		CoreLog::Construct();
		Time::Construct();
		Input::Construct();

		m_Window = specs.WindowSpecs.Build();
		m_RenderEngine = specs.RenderSpecs.Build(m_Window->GetNativeWindow());

		m_ImGUILayer = ImGUILayer::Create();
		PushOverlay(m_ImGUILayer);

		CoreLog::Info("Application Created!");

		Window::OnWindowClose += AX_BIND_FNC(Application::OnWindowClose);
		Window::OnWindowResize += AX_BIND_FNC(Application::OnWindowResize);
	}

	Application::~Application()
	{
		CoreLog::Info("Application Closing...");
		RenderEngine::Destruct();
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

			if (m_ImGUILayer)
			{
				m_ImGUILayer->BeginUI();
				for (Layer* layer : m_LayerStack)
				{
					layer->OnRenderUI();
				}
				m_ImGUILayer->EndUI();
			}

			m_RenderEngine->RenderFrame();
		}
	}

	bool Application::OnWindowClose()
	{
		m_Running = false;
		return false;
	}

	bool Application::OnWindowResize(int width, int height)
	{
		// RendererAPI::SetViewport(width, height);
		return false;
	}
}