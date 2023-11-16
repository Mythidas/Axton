#pragma once

#include "Axton/Core/Defines.h"

namespace Axton
{
	class Bit
	{
	public:
		template <typename T>
		static T Set(T num, uint32_t bit);
		template<typename T, typename VT>
		static T SetTo(T num, VT val, uint32_t bit);
		template <typename T>
		static T Clear(T num, uint32_t bit);
		template <typename T>
		static T Toggle(T num, uint32_t bit);
		template <typename T>
		static bool Check(T num, uint32_t bit);
		template <typename R, typename T>
		static R Get(T num, uint32_t bit);

		static uint32_t U32_4x8(uint8_t x, uint8_t y, uint8_t z, uint8_t w);
	};

	template<typename T>
	inline T Bit::Set(T num, uint32_t bit)
	{
		return num | ((T)1 << bit);
	}

	template<typename T, typename VT>
	inline T Bit::SetTo(T num, VT val, uint32_t bit)
	{
		return (num & (~((VT)1 << bit))) | ((VT)val << bit);
	}

	template<typename T>
	inline T Bit::Clear(T num, uint32_t bit)
	{
		return num & ~((T)1 << bit);
	}

	template<typename T>
	inline T Bit::Toggle(T num, uint32_t bit)
	{
		return num ^ ((T)1 << bit);
	}

	template<typename T>
	inline bool Bit::Check(T num, uint32_t bit)
	{
		return (num >> bit) & (T)1;
	}

	template<typename R, typename T>
	inline R Bit::Get(T num, uint32_t bit)
	{
		return R(num >> bit);
	}
}