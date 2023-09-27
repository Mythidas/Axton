#pragma once

#include "Axton/Core/Defines.h"

namespace Axton
{
	class AX_API Mathf
	{
	public:
		static const float Pi;

		static float ToRadians(float degrees);

		class AX_API Infinity
		{
		public:
			static const float Float;
			static const uint32_t UInt32;
			static const int Int;
		};

		class AX_API Clamp
		{
		public:
			static float Float(float value, float min, float max);
			static int Int(int value, int min, int max);
		};

		class AX_API Random
		{
		public:
			static float Float();
			static float Float(float min, float max);
			static int Int();
			static int Int(int min, int max);
			static uint32_t UInt32();
			static uint32_t UInt32(uint32_t min, uint32_t max);
		};
	};
}