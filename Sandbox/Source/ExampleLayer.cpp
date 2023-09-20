#include "ExampleLayer.h"

void ExampleLayer::OnAttach()
{
	World::SetActiveLevel(CreateRef<Level>());

	Entity cameraEntity = World::CreateEntity();
	Camera* camera = World::AddComponent<Camera>(cameraEntity);
	World::GetComponent<Transform>(cameraEntity)->Position = { 0.0f, 0.0f, -1.0f };

	Entity entity = World::CreateEntity();
	World::GetComponent<Tag>(entity)->Name = "Test";
	SpriteRenderer* spriteRenderer = World::AddComponent<SpriteRenderer>(entity);
	spriteRenderer->Color = { 1.0f, 0.0f, 0.2f, 1.0f };
}

void ExampleLayer::OnUpdate()
{
	World::GetActiveLevel()->OnUpdate();
}