#include "axpch.h"
#include "Application.h"

#include "Axton/Debug/Log.h"
#include "Axton/Event/Events.h"
#include "Axton/Renderer/Renderer2D.h"
#include "Axton/Scene/Level.h"
#include "Axton/Scene/Component.h"
#include "Axton/Scene/EntityView.h"

namespace Axton
{
	Application::Application()
	{
		Log::Construct();
		CoreLog::Construct();

		m_Window = Window::Create();
		Renderer2D::Construct();

		CoreLog::Info("Application Created!");

		Events::OnWindowClose += AX_BIND_FNC(Application::OnWindowClose);
	}

	class TestComponent : public NativeScriptComponent
	{
	public:
		std::string Message = "Hello World!";

		void OnUpdate() override
		{
			CoreLog::Info("TestComponent::OnUpdate {0}", Message);
		}
	};

	void Application::Run()
	{
		Ref<Texture2D> texture = Texture2D::Create(Texture2DSpecs());
		texture->SetData("C:\\Programming\\Axton\\Sandbox\\Assets\\Textures\\test.png");

		Ref<Level> level = CreateRef<Level>();
		Entity ent = level->CreateEntity();
		level->AddComponent<TestComponent>(ent);

		CoreLog::Info(ECSComponent::GetBaseType().c_str());

		while (m_Running)
		{
			m_Window->OnUpdate();

			level->OnUpdate();

			for (const Entity& ent : EntityView<NativeScriptComponent>(level))
			{
				NativeScriptComponent* comp = level->GetComponent<NativeScriptComponent>(ent);
				comp->OnUpdate();
				//CoreLog::Info("Entity {0} has component", comp->GetBaseType());
			}
		}
	}

	void Application::OnWindowClose()
	{
		m_Running = false;

		CoreLog::Info("Application Closing...");
	}
}