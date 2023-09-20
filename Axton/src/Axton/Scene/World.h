#pragma once

#include "Level.h"
#include "Axton/Core/Defines.h"

namespace Axton
{
	class AX_API World
	{
	public:
		static Entity CreateEntity();
		static Entity FindEntity(Entity entity);
		static Entity FindEntity(int index);
		static void DestroyEntity(Entity entity);

		template <typename T>
		static T* AddComponent(Entity entity);
		template <typename T>
		static T* GetComponent(Entity entity);
		template <typename T>
		static void RemoveComponent(Entity entity);

		static Ref<Level> GetActiveLevel() { return s_ActiveLevel; }
		static void SetActiveLevel(const Ref<Level>& level) { s_ActiveLevel = level; }

	private:
		static Ref<Level> s_ActiveLevel;
	};

	template<typename T>
	inline T* World::AddComponent(Entity entity)
	{
		return s_ActiveLevel->AddComponent<T>(entity);
	}

	template<typename T>
	inline T* World::GetComponent(Entity entity)
	{
		return s_ActiveLevel->GetComponent<T>(entity);
	}

	template<typename T>
	inline void World::RemoveComponent(Entity entity)
	{
		s_ActiveLevel->RemoveComponent<T>(entity);
	}
}