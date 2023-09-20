#pragma once

#include "Defines.h"
#include "Window.h"

namespace Axton
{
	class AX_API Application
	{
	public:
		Application();
		virtual ~Application() {}

		void Run();

	private:
		Scope<Window> m_Window;
		bool m_Running = true;

	private:
		void OnWindowClose();
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}