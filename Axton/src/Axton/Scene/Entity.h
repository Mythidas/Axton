#pragma once

#include "Axton/Core/Defines.h"

#include <bitset>

namespace Axton
{
	const int MAX_COMPONENTS = 32;
	const int MAX_ENTITIES = 1000;

	struct EntityID
	{
		int Index;
		int Version;

		EntityID() : Index(-1), Version(-1) {}
		EntityID(int index, int version) : Index(index), Version(version) {}
		EntityID(int index) : Index(index), Version(0) {}

		bool IsValid() const
		{
			return Index != -1;
		}

		bool operator==(const EntityID& other) const
		{
			return IsValid() && other.IsValid() &&
				Index == other.Index && Version == other.Version;
		}
	};

	typedef std::bitset<MAX_COMPONENTS> ComponentMask;

	class AX_API Entity
	{
	public:
		Entity(EntityID id)
		: ID(id), Parent(-1), IsActive(true) {}
		Entity(EntityID id, EntityID parent)
		: ID(id), Parent(parent), IsActive(true) {}
		Entity(EntityID id, EntityID parent, bool active)
		: ID(id), Parent(parent), IsActive(active) {}
		Entity(const Entity& entity)
		: ID(entity.ID), Parent(entity.Parent), Components(entity.Components), IsActive(entity.IsActive) {}

		EntityID ID;
		EntityID Parent;
		ComponentMask Components;
		bool IsActive;

		bool operator==(const Entity& other) const
		{
			return ID == other.ID;
		}
	};

	inline bool operator==(const Ref<Entity> entity, const Entity& id)
	{
		return entity->ID == id.ID;
	}
}