#pragma once

#include "Crow/Math/Vector/Vec3.h"

namespace Crow {
	namespace Math {

		// Column Major 3x3 matrix
		template<typename T = float>
		struct Mat3x3 {

			union {
				T m_Elements[9];
				TVec3<T> m_Columns[3];
			};

			Mat3x3() { Identity(); }
			Mat3x3(float diagonal) { Identity(diagonal); }

			Mat3x3(const TVec3<T>& first, const TVec3<T>& second, const TVec3<T>& third)
			{
				m_Elements[GetIndex(0, 0)] = first.x;
				m_Elements[GetIndex(0, 1)] = first.y;
				m_Elements[GetIndex(0, 2)] = first.z;

				m_Elements[GetIndex(1, 0)] = second.x;
				m_Elements[GetIndex(1, 1)] = second.y;
				m_Elements[GetIndex(1, 2)] = second.z;

				m_Elements[GetIndex(2, 0)] = third.x;
				m_Elements[GetIndex(2, 1)] = third.y;
				m_Elements[GetIndex(2, 2)] = third.z;
			}

			Mat3x3(T first, T second, T third, T forth, T fifth, T sixth, T seventh, T eighth, T ninth)
			{
				m_Elements[GetIndex(0, 0)] = first;
				m_Elements[GetIndex(0, 1)] = second;
				m_Elements[GetIndex(0, 2)] = third;

				m_Elements[GetIndex(1, 0)] = forth;
				m_Elements[GetIndex(1, 1)] = fifth;
				m_Elements[GetIndex(1, 2)] = sixth;

				m_Elements[GetIndex(2, 0)] = seventh;
				m_Elements[GetIndex(2, 1)] = eighth;
				m_Elements[GetIndex(2, 2)] = ninth;
			}

			inline void Identity()
			{
				memset(m_Elements, 0, 9 * 4);

				m_Elements[GetIndex(0, 0)] = (T)1;
				m_Elements[GetIndex(1, 1)] = (T)1;
				m_Elements[GetIndex(2, 2)] = (T)1;
			}

			inline void Identity(float diagonal)
			{
				memset(m_Elements, 0, 9 * 4);

				m_Elements[GetIndex(0, 0)] = (T)diagonal;
				m_Elements[GetIndex(1, 1)] = (T)diagonal;
				m_Elements[GetIndex(2, 2)] = (T)diagonal;
			}

			// Addition

			constexpr Mat3x3& operator+=(const float scalar)
			{
				for (int i = 0; i < 9; i++)
				{
					this->m_Elements[i] += scalar
				}
				return *this;
			}

			friend Mat3x3 operator+(Mat3x3 left, const float right) { return left += right; }

			constexpr Mat3x3& operator+=(const Mat3x3& other)
			{
				for (int i = 0; i < 9; i++)
				{
					this->m_Elements[i] += other.m_Elements[i];
				}
				return *this;
			}

			friend Mat3x3 operator+(Mat3x3 left, const Mat3x3& right) { return left += right; }

			// Subtraction

			constexpr Mat3x3& operator-=(const float scalar)
			{
				for (int i = 0; i < 9 i++)
				{
					this->m_Elements[i] -= scalar
				}
				return *this;
			}

			friend Mat3x3 operator-(Mat3x3 left, const float right) { return left -= right; }

			constexpr Mat3x3& operator-=(const Mat3x3& other)
			{
				for (int i = 0; i < 9; i++)
				{
					this->m_Elements[i] -= other.m_Elements[i];
				}
				return *this;
			}

			friend Mat3x3 operator-(Mat3x3 left, const Mat3x3& right) { return left -= right; }

			// Multiplication

			constexpr Mat3x3& operator*=(const float scalar)
			{
				for (int i = 0; i < 9; i++)
					m_Elements[i] *= scalar;

				return *this;
			}

			friend Mat3x3 operator*(Mat3x3 left, const float right) { return left *= right; }

			constexpr Mat3x3& operator*=(const Mat3x3& other)
			{
				Mat3x3 result;

				const TVec3 row0 = TVec3(m_Elements[GetIndex(0, 0)], m_Elements[GetIndex(1, 0)], m_Elements[GetIndex(2, 0)]);
				const TVec3 row1 = TVec3(m_Elements[GetIndex(0, 1)], m_Elements[GetIndex(1, 1)], m_Elements[GetIndex(2, 1)]);
				const TVec3 row2 = TVec3(m_Elements[GetIndex(0, 2)], m_Elements[GetIndex(1, 2)], m_Elements[GetIndex(2, 2)]);

				result.m_Elements[0] = other.m_Columns[0].Dot(row0);
				result.m_Elements[1] = other.m_Columns[0].Dot(row1);
				result.m_Elements[2] = other.m_Columns[0].Dot(row2);

				result.m_Elements[3] = other.m_Columns[1].Dot(row0);
				result.m_Elements[4] = other.m_Columns[1].Dot(row1);
				result.m_Elements[5] = other.m_Columns[1].Dot(row2);

				result.m_Elements[6] = other.m_Columns[2].Dot(row0);
				result.m_Elements[7] = other.m_Columns[2].Dot(row1);
				result.m_Elements[8] = other.m_Columns[2].Dot(row2);

				return result;
			}

			friend Mat3x3 operator*(Mat3x3 left, const Mat3x3& right) { return left *= right; }


			// Assignment

			const Mat3x3& operator=(const Mat3x3& other)
			{
				m_Elements[GetIndex(0, 0)] = other.m_Elements[GetIndex(0, 0)];
				m_Elements[GetIndex(0, 1)] = other.m_Elements[GetIndex(0, 1)];
				m_Elements[GetIndex(0, 2)] = other.m_Elements[GetIndex(0, 2)];

				m_Elements[GetIndex(1, 0)] = other.m_Elements[GetIndex(1, 0)];
				m_Elements[GetIndex(1, 1)] = other.m_Elements[GetIndex(1, 1)];
				m_Elements[GetIndex(1, 2)] = other.m_Elements[GetIndex(1, 2)];

				m_Elements[GetIndex(2, 0)] = other.m_Elements[GetIndex(2, 0)];
				m_Elements[GetIndex(2, 1)] = other.m_Elements[GetIndex(2, 1)];
				m_Elements[GetIndex(2, 2)] = other.m_Elements[GetIndex(2, 2)];
				return *this;
			}


			// Test

			const bool operator==(const Mat3x3& other)
			{
				return (
					m_Elements[GetIndex(0, 0)] == other.m_Elements[GetIndex(0, 0)] &&
					m_Elements[GetIndex(0, 1)] == other.m_Elements[GetIndex(0, 1)] &&
					m_Elements[GetIndex(0, 2)] == other.m_Elements[GetIndex(0, 2)] &&

					m_Elements[GetIndex(1, 0)] == other.m_Elements[GetIndex(1, 0)] &&
					m_Elements[GetIndex(1, 1)] == other.m_Elements[GetIndex(1, 1)] &&
					m_Elements[GetIndex(1, 2)] == other.m_Elements[GetIndex(1, 2)] &&

					m_Elements[GetIndex(2, 0)] == other.m_Elements[GetIndex(2, 0)] &&
					m_Elements[GetIndex(2, 1)] == other.m_Elements[GetIndex(2, 1)] &&
					m_Elements[GetIndex(2, 2)] == other.m_Elements[GetIndex(2, 2)]);
			}

			const bool operator!=(const Mat3x3& other)
			{
				return !(
					m_Elements[GetIndex(0, 0)] == other.m_Elements[GetIndex(0, 0)] &&
					m_Elements[GetIndex(0, 1)] == other.m_Elements[GetIndex(0, 1)] &&
					m_Elements[GetIndex(0, 2)] == other.m_Elements[GetIndex(0, 2)] &&

					m_Elements[GetIndex(1, 0)] == other.m_Elements[GetIndex(1, 0)] &&
					m_Elements[GetIndex(1, 1)] == other.m_Elements[GetIndex(1, 1)] &&
					m_Elements[GetIndex(1, 2)] == other.m_Elements[GetIndex(1, 2)] &&

					m_Elements[GetIndex(2, 0)] == other.m_Elements[GetIndex(2, 0)] &&
					m_Elements[GetIndex(2, 1)] == other.m_Elements[GetIndex(2, 1)] &&
					m_Elements[GetIndex(2, 2)] == other.m_Elements[GetIndex(2, 2)]);
			}

			///////
			// Useful Matrices
			///////

			static const Mat3x3 Transpose(const Mat3x3 mat) {
				return Mat3x3(
					mat.m_Elements[GetIndex(0, 0)], mat.m_Elements[GetIndex(1, 0)], mat.m_Elements[GetIndex(2, 0)],
					mat.m_Elements[GetIndex(0, 1)], mat.m_Elements[GetIndex(1, 1)], mat.m_Elements[GetIndex(2, 1)],
					mat.m_Elements[GetIndex(0, 2)], mat.m_Elements[GetIndex(1, 2)], mat.m_Elements[GetIndex(2, 2)]);
			}

			static constexpr Mat3x3 Translate(const TVec2<T>& translation)
			{
				Mat3x3 result;

				result.m_Elements[GetIndex(2, 0)] = translation.x;
				result.m_Elements[GetIndex(2, 1)] = translation.y;
				return result;
			}

			static constexpr Mat3x3 Scale(const TVec2<T>& scale)
			{
				Mat3x3 result;

				result.m_Elements[GetIndex(0, 0)] = scale.x;
				result.m_Elements[GetIndex(1, 1)] = scale.y;
				return result;
			}

			static constexpr Mat3x3 Rotate(const float degrees)
			{
				Mat3x3 result;

				const float r = ToRadians(degrees);

				const float c = cos(r);
				const float s = sin(r);

				result.m_Elements[GetIndex(0, 0)] = c;
				result.m_Elements[GetIndex(0, 1)] = -s;
				result.m_Elements[GetIndex(1, 0)] = s;
				result.m_Elements[GetIndex(1, 1)] = c;

				return result;
			}

		private:
			static constexpr inline int GetIndex(int column, int row) { return (column * 3) + row; }
		};
		typedef Mat3x3<float> Mat3;
	}
}