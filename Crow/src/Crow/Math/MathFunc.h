#pragma once



#define D_MATH_PI 3.1415926535897932
#define MATH_PI 3.1415926535897932f

namespace Crow {
	namespace Math {
		
		static float ToRadians(int degrees)
		{
			return degrees * MATH_PI / 180.0f;
		}

		static double ToRadians(double degrees)
		{
			return degrees * D_MATH_PI / 180.0f;
		}

		static constexpr float ToRadians(float degrees)
		{
			return degrees * MATH_PI / 180.0f;
		}

		static double ToDegrees(double radians)
		{
			return radians * 180.0f / D_MATH_PI;
		}

		static float ToDegrees(float radians)
		{
			return radians * 180.0f / MATH_PI;
		}

	}
}