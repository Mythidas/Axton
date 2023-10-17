#pragma once

#include "Axton/Core/Defines.h"

namespace Axton
{
	// TODO: Replace this with something better
	class MemTracker
	{
	public:
		static void SetSlot(const std::string& slot, size_t size);
		static void DeleteSlot(const std::string& slot);
		
		static std::unordered_map<std::string, size_t>::iterator begin() { return m_MemorySlots.begin(); }
		static std::unordered_map<std::string, size_t>::iterator end() { return m_MemorySlots.end(); }

	private:
		static std::unordered_map<std::string, size_t> m_MemorySlots;
	};
}