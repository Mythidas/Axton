#pragma once

#include "Entity.h"
#include "ComponentPool.h"
#include "Axton/Core/Defines.h"

namespace Axton
{
	class AX_API Level
	{
		template <typename... Args>
		friend class EntityView;
	public:
		Level();
		~Level();

		void OnUpdate();

	public:
		Entity CreateEntity();
		Entity FindEntity(Entity entity);
		Entity FindEntity(int index);
		void DestroyEntity(Entity entity);

		template <typename T>
		T* AddComponent(Entity entity);
		template <typename T>
		T* GetComponent(Entity entity);
		template <typename T>
		void RemoveComponent(Entity entity);

		template <typename T>
		int FindComponentID();
		template <typename T>
		std::vector<int> FindComponentIDs(bool isBase = false);

		size_t GetEntityCount() const { return m_Entities.size(); }

	private:
		template <typename T>
		int GetComponentID();

	private:
		std::vector<Ref<Entity>> m_Entities;
		std::vector<EntityID> m_FreeEntities;
		std::vector<ComponentPool*> m_ComponentPools;

		int m_ComponentCount;
	};

	template<typename T>
	inline T* Level::AddComponent(Entity entity)
	{
		if (!entity.ID.IsValid() || m_Entities[entity.ID.Index] != entity) return nullptr;

		int componentID = GetComponentID<T>();

		if (m_ComponentPools.size() <= componentID)
		{
			m_ComponentPools.resize(componentID + 1, nullptr);
		}
		if (m_ComponentPools[componentID] == nullptr)
		{
			m_ComponentPools[componentID] = new ComponentPool(sizeof(T), MAX_ENTITIES, GetTypeName<T>(), T::GetBaseType());
		}

		T* component = new (m_ComponentPools[componentID]->Get(entity.ID.Index)) T();

		m_Entities[entity.ID.Index]->Components.set(componentID);
		return component;
	}

	template<typename T>
	inline T* Level::GetComponent(Entity entity)
	{
		if (!entity.ID.IsValid() || m_Entities[entity.ID.Index] != entity) return nullptr;

		int componentID = FindComponentID<T>();
		if (m_Entities[entity.ID.Index]->Components.test(componentID))
		{
			return static_cast<T*>(m_ComponentPools[componentID]->Get(entity.ID.Index));
		}

		return nullptr;
	}

	template<typename T>
	inline void Level::RemoveComponent(Entity entity)
	{
		if (!entity.ID.IsValid() || m_Entities[entity.ID.Index] != entity) return nullptr;

		int componentID = FindComponentID<T>();
		m_Entities[entity.ID.Index]->Components.reset(componentID);
	}

	template<typename T>
	inline int Level::FindComponentID()
	{
		for (int i = 0; i < m_ComponentCount; i++)
		{
			if (m_ComponentPools[i]->IsType(GetTypeName<T>()))
			{
				return i;
			}
		}

		return -1;
	}

	template<typename T>
	inline std::vector<int> Level::FindComponentIDs(bool isBase)
	{
		std::vector<int> componentIDs;

		if (isBase)
		{
			for (int i = 0; i < m_ComponentCount; i++)
			{
				if (m_ComponentPools[i]->IsType(GetTypeName<T>()) ||
					m_ComponentPools[i]->IsType(T::GetBaseType()))
				{
					componentIDs.push_back(i);
				}
			}
		}
		else
		{
			for (int i = 0; i < m_ComponentCount; i++)
			{
				if (m_ComponentPools[i]->IsType(GetTypeName<T>()))
				{
					componentIDs.push_back(i);
				}
			}
		}

		return componentIDs;
	}

	template<typename T>
	inline int Level::GetComponentID()
	{
		for (int i = 0; i < m_ComponentCount; i++)
		{
			if (m_ComponentPools[i]->IsType(GetTypeName<T>()))
				return i;
		}

		return m_ComponentCount++;
	}
}