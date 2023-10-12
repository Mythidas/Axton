#pragma once

#include "Axton/Core/Defines.h"

namespace Axton
{
	class Mathf
	{
	public:
		static const float Pi;

		static float ToRadians(float degrees);

		template <typename T>
		static T Min(T t1, T t2);
		template <typename T>
		static T Max(T t1, T t2);

		template <typename T>
		static T Clamp(T value, T min, T max);

		class Infinity
		{
		public:
			static const float Float;
			static const uint32_t UInt32;
			static const int Int;
		};

		class Random
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

	template<typename T>
	inline T Mathf::Min(T t1, T t2)
	{
		return t2 < t1 ? t2 : t1;
	}
	template<typename T>
	inline T Mathf::Max(T t1, T t2)
	{
		return t2 > t1 ? t2 : t1;
	}

	template<typename T>
	inline T Mathf::Clamp(T value, T min, T max)
	{
		return value < min ? min : value > max ? max : value;
	}
}