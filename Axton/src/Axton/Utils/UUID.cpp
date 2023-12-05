#include "axpch.h"
#include "UUID.h"
#include "Axton/Math/Mathf.h"

namespace Axton
{
	UUID::UUID()
		: m_UUID(Mathf::Random::UInt64())
	{
		
	}

	UUID::UUID(uint64_t uuid)
		: m_UUID(uuid)
	{

	}
}