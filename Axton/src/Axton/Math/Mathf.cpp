#include "axpch.h"
#include "Mathf.h"

#include <random>

namespace Axton
{
	const float Mathf::Pi = static_cast<float>(3.1415926535897932385);

	const float Mathf::Infinity::Float = std::numeric_limits<float>::infinity();
	const int Mathf::Infinity::Int = std::numeric_limits<int>::infinity();
	const uint32_t Mathf::Infinity::UInt32 = UINT32_MAX;

	float Mathf::ToRadians(float degrees)
	{
		return degrees * Pi / 180.0f;
	}

	float Mathf::Clamp::Float(float value, float min, float max)
	{
		return value < min ? min : value > max ? max : value;
	}

	int Mathf::Clamp::Int(int value, int min, int max)
	{
		return value < min ? min : value > max ? max : value;
	}

	float Mathf::Random::Float()
	{
		static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
		static std::mt19937 generator;
		return distribution(generator);
	}

	float Mathf::Random::Float(float min, float max)
	{
		return min + (max - min) * Random::Float();
	}

	int Mathf::Random::Int()
	{
		return Random::Int(-Infinity::Int, Infinity::Int);
	}

	int Mathf::Random::Int(int min, int max)
	{
		static std::uniform_int_distribution<int> distribution(min, max);
		static std::mt19937 generator;
		return distribution(generator);
	}

	uint32_t Mathf::Random::UInt32()
	{
		return Random::UInt32(0, Infinity::UInt32);
	}

	uint32_t Mathf::Random::UInt32(uint32_t min, uint32_t max)
	{
		static std::uniform_int_distribution<uint32_t> distribution(min, max);
		static std::mt19937 generator;
		return distribution(generator);
	}
}