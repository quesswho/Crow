#pragma once

#include <cmath>

#define D_MATH_PI 3.1415926535897932
#define MATH_PI 3.1415926535897932f

namespace Crow {
	namespace Math {
		
		static inline constexpr float ToRadians(int degrees)
		{
			return degrees * MATH_PI / 180.0f;
		}

		static inline constexpr double ToRadians(double degrees)
		{
			return degrees * D_MATH_PI / 180.0f;
		}

		static inline constexpr float ToRadians(float degrees)
		{
			return degrees * MATH_PI / 180.0f;
		}

		static inline constexpr double ToDegrees(double radians)
		{
			return radians * 180.0f / D_MATH_PI;
		}

		static inline constexpr float ToDegrees(float radians)
		{
			return radians * 180.0f / MATH_PI;
		}

	}
}