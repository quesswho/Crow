#pragma once
#include "Crow/Common.h"

#include "Mat4.h"

namespace Crow {
	namespace Math {

		template<typename T = float>
		static inline const Mat4x4<T> Multiply(const Mat4x4<T>& mat, const Mat4x4<T>& other)
		{
			Mat4x4<T> result;
#ifdef CROW_OGL
			result = mat * other;
#elif defined(CROW_DX11)
			result = other * mat;
#endif
			return result;
		}
		template<typename T = float>
		static inline const Mat4x4<T> Multiply(const Mat4x4<T>& mat, const Mat4x4<T>& other1, const Mat4x4<T>& other2)
		{
			Mat4x4<T> result;
#ifdef CROW_OGL
			result = mat * other1 * other2;
#elif defined(CROW_DX11)
			result = other2 * other1 * mat;
#endif
			return result;
		}
	}
}