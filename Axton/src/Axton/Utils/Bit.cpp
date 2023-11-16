#include "axpch.h"
#include "Bit.h"

namespace Axton
{
	uint32_t Bit::U32_4x8(uint8_t x, uint8_t y, uint8_t z, uint8_t w)
	{
		return uint32_t((w << 24) | (z << 16) | (y << 8) | x);
	}
}