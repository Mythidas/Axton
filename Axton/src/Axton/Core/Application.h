#pragma once

#include "Defines.h"
#include "Window.h"
#include "LayerStack.h"

namespace Axton
{
	class AX_API Application
	{
	public:
		Application();
		virtual ~Application() {}

		void Run();

		void PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); }
		void PushOverlay(Layer* overlay) { m_LayerStack.PushOverlay(overlay); }

		void PopLayer(Layer* layer) { m_LayerStack.PopLayer(layer); }
		void PopOverlay(Layer* overlay) { m_LayerStack.PopOverlay(overlay); }

	private:
		Scope<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

	private:
		void OnWindowClose();
		void OnWindowResize(int width, int height);
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}