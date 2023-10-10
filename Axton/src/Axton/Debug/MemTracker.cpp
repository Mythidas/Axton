#include "axpch.h"
#include "MemTracker.h"

namespace Axton
{
	std::unordered_map<std::string, size_t> MemTracker::m_MemorySlots;

	void MemTracker::SetSlot(const std::string& slot, size_t size)
	{
		m_MemorySlots[slot] = size;
	}

	void MemTracker::DeleteSlot(const std::string& slot)
	{
		m_MemorySlots.erase(slot);
	}
}