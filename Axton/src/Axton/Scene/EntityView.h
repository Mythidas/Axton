#pragma once

#include "Level.h"
#include "Axton/Core/Defines.h"

namespace Axton
{
	template <typename... Components>
	class EntityView
	{
	public:
		EntityView(Ref<Level> level)
			: m_Level(level)
		{
			if (sizeof...(Components) == 0)
			{
				m_All = true;
			}
			else
			{
				int componentIds[] = { 0, m_Level->FindComponentID<Components>()... };
				for (int i = 1; i < sizeof...(Components) + 1; i++)
				{
					if (componentIds[i] != -1)
						m_Mask.set(componentIds[i]);
				}
			}
		}

		struct Iterator
		{
			Iterator(int index, Ref<Level> level, ComponentMask mask, bool all)
				: index(index), level(level), mask(mask), all(all)
			{
			}

			Entity operator*()
			{
				return level->FindEntity(index);
			}

			bool operator==(const Iterator& other) const
			{
				return index == other.index || index == level->GetEntityCount();
			}

			bool operator!=(const Iterator& other) const
			{
				return index != other.index || index != level->GetEntityCount();
			}

			Iterator& operator++()
			{
				do
				{
					index++;
				} while (index < level->GetEntityCount() && !ValidIndex());
				return *this;
			}

			bool ValidIndex()
			{
				Entity ent = level->FindEntity(index);
				return ent.ID.IsValid() && (all || mask == (mask & ent.Components));
			}

			Ref<Level> level;
			int index{ 0 };
			bool all{ false };
			ComponentMask mask;
		};

		const Iterator begin() const
		{
			int firstIndex = 0;
			Entity ent = m_Level->FindEntity(firstIndex);
			while (!m_All && firstIndex < m_Level->GetEntityCount() &&
				(m_Mask != ent.Components || !ent.ID.IsValid()))
			{
				firstIndex++;
			}

			if (firstIndex == 0 && m_Mask != ent.Components && !m_All)
				return end();

			return Iterator(firstIndex, m_Level, m_Mask, m_All);
		}

		const Iterator end() const
		{
			return Iterator(m_Level->GetEntityCount(), m_Level, m_Mask, m_All);
		}

	private:
		Ref<Level> m_Level{ nullptr };
		bool m_All{ false };
		ComponentMask m_Mask;
	};
}