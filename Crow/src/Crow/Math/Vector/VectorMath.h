#pragma once

#include "Vec4.h"
#include "Vec2.h"
#include "Vec3.h"

namespace Crow {

	namespace Math {
		// Normalize a Vec3
		static const Vec2<float> Normalize(const Vec2<float>& vec)
		{
			float mag = vec.Magnitude();
			if (mag > 0)
				return vec * (1.0f / mag);
			return vec;
		}

		// Dot product 
		static inline float Dot(const Vec2<float>& first, const Vec2<float>& second)
		{
			return first.x * first.x + first.y * first.y;
		}

		// Normalize a Vec3
		static const Vec3<float> Normalize(const Vec3<float>& vec)
		{
			float mag = vec.Magnitude();
			if (mag > 0)
				return vec * (1.0f / mag);
			return vec;
		}

		// Dot product 
		static inline float Dot(const Vec3<float>& first, const Vec3<float>& second)
		{
			return first.x * second.x + first.y * second.y + first.z * second.z;
		}

		// Normalize and negate a Vec3
		static inline const Vec3<float> NegativeNormalize(const Vec3<float>& vec)
		{
			float mag = vec.Magnitude();
			if (mag > 0)
				return vec * -(1.0f / mag);
			return vec;
		}

		// Cross product
		static inline Vec3<float> Cross(const Vec3<float>& first, const Vec3<float>& second)
		{
			return Vec3<float>(first.y * second.z - first.z * second.y, first.z * second.x - first.x * second.z, first.x * second.y - first.y * second.x);
		}

		// Normalize a Vec4
		inline const Vec4<float> Normalize(const Vec4<float>& vec)
		{
			float mag = vec.Magnitude();
			if (mag > 0)
				return vec * (1 / mag);
			return vec;
		}

		// Dot product of two Vec4
		static float Dot(const Vec4<float>& first, const Vec4<float>& second)
		{
			return first.x * second.x + first.y * second.y + first.z * second.z + first.w * second.w;
		}

		// Get distance between this vector and other vector
		static inline float Distance(const Vec4<float>& first, const Vec4<float>& second)
		{
			return sqrt(
				(first.x - second.x) * (first.x - second.x) +
				(first.y - second.y) * (first.y - second.y) +
				(first.z - second.z) * (first.z - second.z) +
				(first.w - second.w) * (first.w - second.w));
		}
	}
}