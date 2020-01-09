#pragma once

// Header including all math headers

#include "Matrix/Mat2.h"
#include "Matrix/Mat3.h"
#include "Matrix/Mat4.h"
#include "Vector/VectorMath.h"


namespace Crow {
	namespace Math {
		class MATH_COORDINATE
		{
		public:
			typedef enum MATH_COORDINATE_TYPE
			{
				MATH_COORDINATE_RIGHTHAND = 0x1L,
				MATH_COORDINATE_LEFTHAND = 0x2L
			} 	MATH_COORDINATE_TYPE;

			static MATH_COORDINATE_TYPE s_MathCoordinateType;

		};
	}
}