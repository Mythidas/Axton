#pragma once

#include "Defines.h"
#include "Window.h"
#include "LayerStack.h"
#include "Axton/Renderer/RenderEngine.h"

namespace Axton
{
	class ImGUILayer;

	class Application
	{
	public:
		struct Specs
		{
			Window::Specs WindowSpecs;
			RenderEngine::Specs RenderSpecs;

			Specs& setWindowSpecs(const Window::Specs& specs) { WindowSpecs = specs; return *this; }
			Specs& setRenderSpecs(const RenderEngine::Specs& specs) { RenderSpecs = specs; return *this; }
			// Application* Build() { return new Application(*this); } Application only built through CreateApplication;
		};

		Application(const Specs& specs);
		virtual ~Application();

		void Run();

		void PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); }
		void PushOverlay(Layer* overlay) { m_LayerStack.PushOverlay(overlay); }

		void PopLayer(Layer* layer) { m_LayerStack.PopLayer(layer); }
		void PopOverlay(Layer* overlay) { m_LayerStack.PopOverlay(overlay); }

		static Scope<Window>& GetWindow() { return s_Instance->m_Window; }
		static Scope<RenderEngine>& GetRenderEngine() { return s_Instance->m_RenderEngine; }
		static ImGUILayer& GetImGUILayer() { return *s_Instance->m_ImGUILayer; }

		static Application& Get() { return *s_Instance; }

	private:
		static Application* s_Instance;

		Scope<Window> m_Window;
		Scope<RenderEngine> m_RenderEngine;
		bool m_Running = true;
		LayerStack m_LayerStack;

		ImGUILayer* m_ImGUILayer;

	private:
		bool OnWindowClose();
		bool OnWindowResize(int width, int height);
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}