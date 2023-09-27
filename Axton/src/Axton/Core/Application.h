#pragma once

#include "Defines.h"
#include "Window.h"
#include "LayerStack.h"

namespace Axton
{
	class ImGUILayer;

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

		Window& GetWindow() { return *m_Window; }

		static Application& Get() { return *s_Instance; }

	private:
		static Application* s_Instance;

		Scope<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		ImGUILayer* m_ImGUILayer;

	private:
		void OnWindowClose();
		void OnWindowResize(int width, int height);
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}