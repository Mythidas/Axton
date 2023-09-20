#include "axpch.h"
#include "World.h"

namespace Axton
{
	Ref<Level> World::s_ActiveLevel = nullptr;

	Entity World::CreateEntity()
	{
		return s_ActiveLevel->CreateEntity();
	}
	Entity World::FindEntity(Entity entity)
	{
		return s_ActiveLevel->FindEntity(entity);
	}
	Entity World::FindEntity(int index)
	{
		return s_ActiveLevel->FindEntity(index);
	}
	void World::DestroyEntity(Entity entity)
	{
		s_ActiveLevel->DestroyEntity(entity);
	}
}