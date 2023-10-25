#pragma once

#include <glm/glm.hpp>

namespace Axton
{
	typedef glm::vec2 Vector2;
	typedef glm::vec3 Vector3;
	typedef glm::vec4 Vector4;

	typedef glm::ivec3 IVector3;
	typedef glm::ivec4 IVector4;

	typedef glm::uvec3 UVector3;

	class Vector
	{
	public:
		// Vector3
		static Vector3 Random();
		static Vector3 Random(float min, float max);

		static Vector3 UnitVector(const Vector3& v1);
		static Vector3 RandomUnitVector();
		static Vector3 RandomInUnitSphere();
		static Vector3 RandomInUnitDisk();
		static Vector3 RandomOnHemisphere(const Vector3& normal);

		static Vector3 Reflect(const Vector3& v1, const Vector3& v2);
		static Vector3 Refract(const Vector3& uv, const Vector3& norm, float etaiOverEtat);

		static Vector3 Cross(const Vector3& v1, const Vector3& v2);
		static Vector3 Abs(const Vector3& v1);

		static float Dot(const Vector3& v1, const Vector3& v2);
		static float Length(const Vector3& v1);
		static float Length(const Vector3& v1, const Vector3& v2);
		static float Magnitude(const Vector3& v1);

		static bool NearZero(const Vector3& v1);

		// IVector3
		static IVector3 Clamp(const IVector3& v1, const IVector3& min, const IVector3& max);

		//Vector4
		static Vector4 Clamp(const Vector4& v1, const Vector4& min, const Vector4& max);

		static Vector4 ConvertV4(uint32_t value);

		// IVector4
		static IVector4 ConvertIV4(uint32_t value);
	};
}