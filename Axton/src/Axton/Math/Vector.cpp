#include "axpch.h"
#include "Vector.h"
#include "Mathf.h"
#include "Bit.h"

namespace Axton
{
	Vector3 Vector::Random()
	{
		return Vector3(Mathf::Random::Float(), Mathf::Random::Float(), Mathf::Random::Float());
	}

	Vector3 Vector::Random(float min, float max)
	{
		return Vector3(Mathf::Random::Float(min, max), Mathf::Random::Float(min, max), Mathf::Random::Float(min, max));
	}

	Vector3 Vector::UnitVector(const Vector3& v1)
	{
		return v1 / Length(v1, v1);
	}

	Vector3 Vector::RandomUnitVector()
	{
		return UnitVector(RandomInUnitSphere());
	}

	Vector3 Vector::RandomInUnitSphere()
	{
		while (true)
		{
			Vector3 v1 = Random(-1.0f, 1.0f);
			if (Dot(v1, v1) < 1.0f)
				return v1;
		}
	}

	Vector3 Vector::RandomInUnitDisk()
	{
		while (true)
		{
			Vector3 v1 = Vector3(Mathf::Random::Float(-1.0f, 1.0f), Mathf::Random::Float(-1.0f, 1.0f), 0.0f);
			if (Dot(v1, v1) < 1.0f)
				return v1;
		}
	}

	Vector3 Vector::RandomOnHemisphere(const Vector3& normal)
	{
		Vector3 onUnitSphere = RandomUnitVector();
		if (Dot(onUnitSphere, normal) > 0.0f)
		{
			return onUnitSphere;
		}
		else
		{
			return -onUnitSphere;
		}
	}

	Vector3 Vector::Reflect(const Vector3& v1, const Vector3& v2)
	{
		return v1 - 2.0f * Dot(v1, v2) * v2;
	}

	Vector3 Vector::Refract(const Vector3& uv, const Vector3& norm, float etaiOverEtat)
	{
		float cosTheta = fmin(Dot(-uv, norm), 1.0f);
		Vector3 rOutPerp = etaiOverEtat * (uv + cosTheta * norm);
		Vector3 rOutPara = -sqrt(fabs(1.0f - Dot(rOutPerp, rOutPerp))) * norm;
		return rOutPerp + rOutPara;
	}

	Vector3 Vector::Cross(const Vector3& v1, const Vector3& v2)
	{
		return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
	}

	Vector3 Vector::Abs(const Vector3& v1)
	{
		return Vector3(abs(v1.x), abs(v1.y), abs(v1.z));
	}

	float Vector::Dot(const Vector3& v1, const Vector3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	float Vector::Length(const Vector3& v1)
	{
		return sqrt(Dot(v1, v1));
	}

	float Vector::Length(const Vector3& v1, const Vector3& v2)
	{
		return sqrt(Dot(v1, v2));
	}

	float Vector::Magnitude(const Vector3& v1)
	{
		return v1.x * v1.y * v1.z;
	}

	bool Vector::NearZero(const Vector3& v1)
	{
		auto zero = 1e-8;
		return fabs(v1.x) < zero && fabs(v1.y) < zero && fabs(v1.z) < zero;
	}

	Vector4 Vector::Clamp(const Vector4& v1, const Vector4& min, const Vector4& max)
	{
		return Vector4(v1.x < min.x ? min.x : v1.x > max.x ? max.x : v1.x, v1.y < min.y ? min.y : v1.y > max.y ? max.y : v1.y,
			v1.z < min.z ? min.z : v1.z > max.z ? max.z : v1.z, v1.w < min.w ? min.w : v1.w > max.w ? max.w : v1.w);
	}

	IVector4 Vector::ConvertIV4(uint32_t value)
	{
		return IVector4(Bit::Get<uint8_t>(value, 0), Bit::Get<uint8_t>(value, 8),
			Bit::Get<uint8_t>(value, 16), Bit::Get<uint8_t>(value, 24));
	}

	Vector4 Vector::ConvertV4(uint32_t value)
	{
		return Vector4(Bit::Get<uint8_t>(value, 0), Bit::Get<uint8_t>(value, 8),
			Bit::Get<uint8_t>(value, 16), Bit::Get<uint8_t>(value, 24));
	}
}