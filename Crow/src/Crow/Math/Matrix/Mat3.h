#include "Crow/Math/MathFunc.h"
#include "Crow/Math/Vector/Vec3.h"

namespace Crow {
	namespace Math
	{

		// Column Major 3x3 matrix
		template<typename T = float>
		struct Mat3 {

			union {
				T m_Elements[9];
				Vec3<T> m_Columns[3];
			};

			Mat3() { Identity(); }
			Mat3(float diagonal) { Identity(diagonal) }

			Mat3(const Vec3<T>& first, const Vec3<T>& second, const Vec3<T>& third)
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

			Mat3(T first, T second, T third, T forth, T fifth, T sixth, T seventh, T eighth, T ninth)
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

			constexpr Mat3& operator+=(const float scalar)
			{
				for (int i = 0; i < 9; i++)
				{
					this->m_Elements[i] += scalar
				}
				return *this;
			}

			friend Mat3 operator+(Mat3 left, const float right) { return left += right; }

			constexpr Mat3& operator+=(const Mat3& other)
			{
				for (int i = 0; i < 9; i++)
				{
					this->m_Elements[i] += other.m_Elements[i];
				}
				return *this;
			}

			friend Mat3 operator+(Mat3 left, const Mat3& right) { return left += right; }

			// Subtraction

			constexpr Mat3& operator-=(const float scalar)
			{
				for (int i = 0; i < 9 i++)
				{
					this->m_Elements[i] -= scalar
				}
				return *this;
			}

			friend Mat3 operator-(Mat3 left, const float right) { return left -= right; }

			constexpr Mat3& operator-=(const Mat3& other)
			{
				for (int i = 0; i < 9; i++)
				{
					this->m_Elements[i] -= other.m_Elements[i];
				}
				return *this;
			}

			friend Mat3 operator-(Mat3 left, const Mat3& right) { return left -= right; }

			// Multiplication

			constexpr Mat3& operator*=(const float scalar)
			{
				for (int i = 0; i < 9; i++)
					m_Elements[i] *= scalar;

				return *this;
			}

			friend Mat3 operator*(Mat3 left, const float right) { return left *= right; }

			constexpr Mat3& operator*=(const Mat3& other)
			{
				int r = 0;
				for (int i = 0; i < 9; i++)
				{
					r = i % 3;
					m_Elements[i] = other.m_Columns[(int)i / 3].Dot(m_Elements[GetIndex(0, r)], m_Elements[GetIndex(1, r)], m_Elements[GetIndex(2, r)]);

				}
				return *this;
			}

			friend Mat3 operator*(Mat3 left, const Mat3& right) { return left *= right; }


			// Assignment

			const Mat3& operator=(const Mat3& other)
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

			const bool operator==(const Mat3& other)
			{
				return (m_Elements[GetIndex(0, 0)] == other.m_Elements[GetIndex(0, 0)] &&
					m_Elements[GetIndex(0, 1)] == other.m_Elements[GetIndex(0, 1)] &&
					m_Elements[GetIndex(0, 2)] == other.m_Elements[GetIndex(0, 2)] &&

					m_Elements[GetIndex(1, 0)] == other.m_Elements[GetIndex(1, 0)] &&
					m_Elements[GetIndex(1, 1)] == other.m_Elements[GetIndex(1, 1)] &&
					m_Elements[GetIndex(1, 2)] == other.m_Elements[GetIndex(1, 2)] &&

					m_Elements[GetIndex(2, 0)] == other.m_Elements[GetIndex(2, 0)] &&
					m_Elements[GetIndex(2, 1)] == other.m_Elements[GetIndex(2, 1)] &&
					m_Elements[GetIndex(2, 2)] == other.m_Elements[GetIndex(2, 2)]);
			}

			const bool operator!=(const Mat3& other)
			{
				return !(m_Elements[GetIndex(0, 0)] == other.m_Elements[GetIndex(0, 0)] &&
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

			static constexpr Mat3 Translate(const Vec2<T>& translation)
			{
				Mat3 result;

				result.m_Elements[GetIndex(2, 0)] = translation.x;
				result.m_Elements[GetIndex(2, 1)] = translation.y;
				return result;
			}

			static constexpr Mat3 Scale(const Vec2<T>& scale)
			{
				Mat3 result;

				result.m_Elements[GetIndex(0, 0)] = scale.x;
				result.m_Elements[GetIndex(1, 1)] = scale.y;
				return result;
			}

			static constexpr Mat3 Rotate(const float degrees)
			{
				Mat3 result;

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
	}
}