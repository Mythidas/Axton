#include "axpch.h"
#include "Level.h"
#include "World.h"
#include "EntityView.h"
#include "StandardComponents.h"
#include "Axton/Renderer/Renderer2D.h"

namespace Axton
{
	Level::Level()
		: m_ComponentCount(0)
	{
	}

	Level::~Level()
	{
		//for (auto* pool : m_ComponentPools)
		//{
		//	delete pool;
		//}
	}

	void Level::OnUpdate()
	{
		Camera* camera = nullptr;
		for (const Entity& entity : EntityView<Camera>(World::GetActiveLevel()))
		{
			camera = GetComponent<Camera>(entity);
			Transform* transform = GetComponent<Transform>(entity);

			camera->Data.RecalculateViewMatrix(transform->Position);
			camera->Data.RecalculateProjectionMatrix();
			break;
		}
		if (!camera) return;

		Renderer2D::BeginFrame(camera->Data);

		for (const Entity& entity : EntityView<SpriteRenderer>(World::GetActiveLevel()))
		{
			Transform* transform = GetComponent<Transform>(entity);
			SpriteRenderer* spriteRenderer = GetComponent<SpriteRenderer>(entity);

			if (spriteRenderer->Sprite)
			{
				Renderer2D::DrawRotateQuad(transform->Position, transform->Rotation, transform->Scale,
					spriteRenderer->Color, spriteRenderer->Sprite);
			}
			else
			{
				Renderer2D::DrawRotateQuad(transform->Position, transform->Rotation, transform->Scale,
					spriteRenderer->Color);
			}
		}

		Renderer2D::EndFrame();
	}

	Entity Level::CreateEntity()
	{
		if (!m_FreeEntities.empty())
		{
			EntityID newID = m_FreeEntities.back();
			m_FreeEntities.pop_back();
			newID.Version = m_Entities[newID.Index]->ID.Version;
			m_Entities[newID.Index]->ID.Index = newID.Index;
			AddComponent<Tag>(*m_Entities[newID.Index]);
			AddComponent<Transform>(*m_Entities[newID.Index]);
			return Entity(newID);
		}

		m_Entities.push_back(CreateRef<Entity>(EntityID(m_Entities.size(), 0)));
		AddComponent<Tag>(*m_Entities.back());
		AddComponent<Transform>(*m_Entities.back());
		return Entity(m_Entities.back()->ID);
	}

	Entity Level::FindEntity(Entity entity)
	{
		if (!entity.ID.IsValid()) return Entity(-1);

		if (entity.ID.Index < m_Entities.size())
		{
			if (m_Entities[entity.ID.Index]->ID.Index == entity.ID.Index)
			{
				return Entity(*m_Entities[entity.ID.Index]);
			}
		}

		return Entity(-1);
	}

	Entity Level::FindEntity(int index)
	{
		if (index == -1) return Entity(-1);
		
		if (index < m_Entities.size())
		{
			if (m_Entities[index]->ID.Index == index)
			{
				return Entity(*m_Entities[index]);
			}
		}

		return Entity(-1);
	}

	void Level::DestroyEntity(Entity entity)
	{
		if (m_Entities[entity.ID.Index] == entity)
		{
			m_Entities[entity.ID.Index]->ID.Version++;
			m_Entities[entity.ID.Index]->ID.Index = -1;
			m_Entities[entity.ID.Index]->Components.reset();
			m_FreeEntities.push_back(entity.ID);
		}
	}
}