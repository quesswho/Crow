#pragma once

#include <cstring>

#include "Crow/Math/MathFunc.h"
#include "Crow/Math/Matrix/Mat3.h"
#include "Crow/Math/Vector/Vec4.h"

namespace Crow {
	namespace Math {


		// Column Major 4x4 matrix
		template<typename T = float>
		struct Mat4x4 {

			union {
				T m_Elements[16];
				TVec4<T> m_Columns[4];
			};

			Mat4x4() { Identity(); }
			Mat4x4(float diagonal) { Identity(diagonal); }

			Mat4x4(const TVec4<T>& first, const TVec4<T>& second, const TVec4<T>& third, const TVec4<T>& forth)
			{
				m_Elements[GetIndex(0, 0)] = first.x;
				m_Elements[GetIndex(0, 1)] = first.y;
				m_Elements[GetIndex(0, 2)] = first.z;
				m_Elements[GetIndex(0, 3)] = first.w;

				m_Elements[GetIndex(1, 0)] = second.x;
				m_Elements[GetIndex(1, 1)] = second.y;
				m_Elements[GetIndex(1, 2)] = second.z;
				m_Elements[GetIndex(1, 3)] = second.w;

				m_Elements[GetIndex(2, 0)] = third.x;
				m_Elements[GetIndex(2, 1)] = third.y;
				m_Elements[GetIndex(2, 2)] = third.z;
				m_Elements[GetIndex(2, 3)] = third.w;

				m_Elements[GetIndex(3, 0)] = forth.x;
				m_Elements[GetIndex(3, 1)] = forth.y;
				m_Elements[GetIndex(3, 2)] = forth.z;
				m_Elements[GetIndex(3, 3)] = forth.w;
			}

			Mat4x4(T first, T second, T third, T forth, T fifth, T sixth, T seventh, T eighth, T ninth, T tenth, T eleventh, T twelveth, T thirteenth, T fourteenth, T fifteenth, T sixteenth)
			{
				m_Elements[GetIndex(0, 0)] = first;
				m_Elements[GetIndex(0, 1)] = second;
				m_Elements[GetIndex(0, 2)] = third;
				m_Elements[GetIndex(0, 3)] = forth;

				m_Elements[GetIndex(1, 0)] = fifth;
				m_Elements[GetIndex(1, 1)] = sixth;
				m_Elements[GetIndex(1, 2)] = seventh;
				m_Elements[GetIndex(1, 3)] = eighth;

				m_Elements[GetIndex(2, 0)] = ninth;
				m_Elements[GetIndex(2, 1)] = tenth;
				m_Elements[GetIndex(2, 2)] = eleventh;
				m_Elements[GetIndex(2, 3)] = twelveth;

				m_Elements[GetIndex(3, 0)] = thirteenth;
				m_Elements[GetIndex(3, 1)] = fourteenth;
				m_Elements[GetIndex(3, 2)] = fifteenth;
				m_Elements[GetIndex(3, 3)] = sixteenth;
			}

			inline void Identity()
			{
				memset(m_Elements, 0, 16 * 4);

				m_Elements[GetIndex(0, 0)] = (T)1;
				m_Elements[GetIndex(1, 1)] = (T)1;
				m_Elements[GetIndex(2, 2)] = (T)1;
				m_Elements[GetIndex(3, 3)] = (T)1;
			}

			inline void Identity(float diagonal)
			{
				memset(m_Elements, 0, 16 * 4);

				m_Elements[GetIndex(0, 0)] = (T)diagonal;
				m_Elements[GetIndex(1, 1)] = (T)diagonal;
				m_Elements[GetIndex(2, 2)] = (T)diagonal;
				m_Elements[GetIndex(3, 3)] = (T)diagonal;
			}


			// Addition

			constexpr Mat4x4& operator+=(const float scalar)
			{
				for (int i = 0; i < 16; i++)
				{
					this->m_Elements[i] += scalar
				}
				return *this;
			}

			friend Mat4x4 operator+(Mat4x4 left, const float right) { return left += right; }

			constexpr Mat4x4& operator+=(const Mat4x4& other)
			{
				for (int i = 0; i < 16; i++)
				{
					this->m_Elements[i] += other.m_Elements[i];
				}
				return *this;
			}

			friend Mat4x4 operator+(Mat4x4 left, const Mat4x4& right) { return left += right; }

			// Subtraction

			constexpr Mat4x4& operator-=(const float scalar)
			{
				for (int i = 0; i < 16; i++)
				{
					this->m_Elements[i] -= scalar
				}
				return *this;
			}

			friend Mat4x4 operator-(Mat4x4 left, const float right) { return left -= right; }

			constexpr Mat4x4& operator-=(const Mat4x4& other)
			{
				for (int i = 0; i < 16; i++)
				{
					this->m_Elements[i] -= other.m_Elements[i];
				}
				return *this;
			}

			friend Mat4x4 operator-(Mat4x4 left, const Mat4x4& right) { return left -= right; }

			// Multiplication
			
			constexpr Mat4x4& operator*=(const float scalar)
			{
				for (int i = 0; i < 16; i++)
					m_Elements[i] *= scalar;

				return *this;
			}

			friend Mat4x4 operator*(Mat4x4 left, const float right) { return left *= right; }


			inline Mat4x4& operator*=(const Mat4x4& other)
			{
				return Mat4x4(
					other.m_Columns[0].Dot(m_Elements[GetIndex(0, 0)], m_Elements[GetIndex(1, 0)], m_Elements[GetIndex(2, 0)], m_Elements[GetIndex(3, 0)]),
					other.m_Columns[0].Dot(m_Elements[GetIndex(0, 1)], m_Elements[GetIndex(1, 1)], m_Elements[GetIndex(2, 1)], m_Elements[GetIndex(3, 1)]),
					other.m_Columns[0].Dot(m_Elements[GetIndex(0, 2)], m_Elements[GetIndex(1, 2)], m_Elements[GetIndex(2, 2)], m_Elements[GetIndex(3, 2)]),
					other.m_Columns[0].Dot(m_Elements[GetIndex(0, 3)], m_Elements[GetIndex(1, 3)], m_Elements[GetIndex(2, 3)], m_Elements[GetIndex(3, 3)]),
					
					other.m_Columns[1].Dot(m_Elements[GetIndex(0, 0)], m_Elements[GetIndex(1, 0)], m_Elements[GetIndex(2, 0)], m_Elements[GetIndex(3, 0)]),
					other.m_Columns[1].Dot(m_Elements[GetIndex(0, 1)], m_Elements[GetIndex(1, 1)], m_Elements[GetIndex(2, 1)], m_Elements[GetIndex(3, 1)]),
					other.m_Columns[1].Dot(m_Elements[GetIndex(0, 2)], m_Elements[GetIndex(1, 2)], m_Elements[GetIndex(2, 2)], m_Elements[GetIndex(3, 2)]),
					other.m_Columns[1].Dot(m_Elements[GetIndex(0, 3)], m_Elements[GetIndex(1, 3)], m_Elements[GetIndex(2, 3)], m_Elements[GetIndex(3, 3)]),

					other.m_Columns[2].Dot(m_Elements[GetIndex(0, 0)], m_Elements[GetIndex(1, 0)], m_Elements[GetIndex(2, 0)], m_Elements[GetIndex(3, 0)]),
					other.m_Columns[2].Dot(m_Elements[GetIndex(0, 1)], m_Elements[GetIndex(1, 1)], m_Elements[GetIndex(2, 1)], m_Elements[GetIndex(3, 1)]),
					other.m_Columns[2].Dot(m_Elements[GetIndex(0, 2)], m_Elements[GetIndex(1, 2)], m_Elements[GetIndex(2, 2)], m_Elements[GetIndex(3, 2)]),
					other.m_Columns[2].Dot(m_Elements[GetIndex(0, 3)], m_Elements[GetIndex(1, 3)], m_Elements[GetIndex(2, 3)], m_Elements[GetIndex(3, 3)]),

					other.m_Columns[3].Dot(m_Elements[GetIndex(0, 0)], m_Elements[GetIndex(1, 0)], m_Elements[GetIndex(2, 0)], m_Elements[GetIndex(3, 0)]),
					other.m_Columns[3].Dot(m_Elements[GetIndex(0, 1)], m_Elements[GetIndex(1, 1)], m_Elements[GetIndex(2, 1)], m_Elements[GetIndex(3, 1)]),
					other.m_Columns[3].Dot(m_Elements[GetIndex(0, 2)], m_Elements[GetIndex(1, 2)], m_Elements[GetIndex(2, 2)], m_Elements[GetIndex(3, 2)]),
					other.m_Columns[3].Dot(m_Elements[GetIndex(0, 3)], m_Elements[GetIndex(1, 3)], m_Elements[GetIndex(2, 3)], m_Elements[GetIndex(3, 3)])
					);
			}

			friend Mat4x4 operator*(Mat4x4 left, const Mat4x4& right) { return left *= right; }


			// Assignment

			const Mat4x4& operator=(const Mat4x4& other)
			{
				m_Elements[GetIndex(0, 0)] = other.m_Elements[GetIndex(0, 0)];
				m_Elements[GetIndex(0, 1)] = other.m_Elements[GetIndex(0, 1)];
				m_Elements[GetIndex(0, 2)] = other.m_Elements[GetIndex(0, 2)];
				m_Elements[GetIndex(0, 3)] = other.m_Elements[GetIndex(0, 3)];

				m_Elements[GetIndex(1, 0)] = other.m_Elements[GetIndex(1, 0)];
				m_Elements[GetIndex(1, 1)] = other.m_Elements[GetIndex(1, 1)];
				m_Elements[GetIndex(1, 2)] = other.m_Elements[GetIndex(1, 2)];
				m_Elements[GetIndex(1, 3)] = other.m_Elements[GetIndex(1, 3)];

				m_Elements[GetIndex(2, 0)] = other.m_Elements[GetIndex(2, 0)];
				m_Elements[GetIndex(2, 1)] = other.m_Elements[GetIndex(2, 1)];
				m_Elements[GetIndex(2, 2)] = other.m_Elements[GetIndex(2, 2)];
				m_Elements[GetIndex(2, 3)] = other.m_Elements[GetIndex(2, 3)];
				
				m_Elements[GetIndex(3, 0)] = other.m_Elements[GetIndex(3, 0)];
				m_Elements[GetIndex(3, 1)] = other.m_Elements[GetIndex(3, 1)];
				m_Elements[GetIndex(3, 2)] = other.m_Elements[GetIndex(3, 2)];
				m_Elements[GetIndex(3, 3)] = other.m_Elements[GetIndex(3, 3)];
				return *this;
			}

			// Test

			const bool operator==(const Mat4x4& other)
			{
				return (m_Elements[GetIndex(0, 0)] == other.m_Elements[GetIndex(0, 0)] &&
					m_Elements[GetIndex(0, 1)] == other.m_Elements[GetIndex(0, 1)] &&
					m_Elements[GetIndex(0, 2)] == other.m_Elements[GetIndex(0, 2)] &&
					m_Elements[GetIndex(0, 3)] == other.m_Elements[GetIndex(0, 3)] &&

					m_Elements[GetIndex(1, 0)] == other.m_Elements[GetIndex(1, 0)] &&
					m_Elements[GetIndex(1, 1)] == other.m_Elements[GetIndex(1, 1)] &&
					m_Elements[GetIndex(1, 2)] == other.m_Elements[GetIndex(1, 2)] &&
					m_Elements[GetIndex(1, 3)] == other.m_Elements[GetIndex(1, 3)] &&

					m_Elements[GetIndex(2, 0)] == other.m_Elements[GetIndex(2, 0)] &&
					m_Elements[GetIndex(2, 1)] == other.m_Elements[GetIndex(2, 1)] &&
					m_Elements[GetIndex(2, 2)] == other.m_Elements[GetIndex(2, 2)] &&
					m_Elements[GetIndex(2, 3)] == other.m_Elements[GetIndex(2, 3)] &&

					m_Elements[GetIndex(3, 0)] == other.m_Elements[GetIndex(3, 0)] &&
					m_Elements[GetIndex(3, 1)] == other.m_Elements[GetIndex(3, 1)] &&
					m_Elements[GetIndex(3, 2)] == other.m_Elements[GetIndex(3, 2)] &&
					m_Elements[GetIndex(3, 3)] == other.m_Elements[GetIndex(3, 3)]);
			}

			const bool operator!=(const Mat4x4& other)
			{
				return !(m_Elements[GetIndex(0, 0)] == other.m_Elements[GetIndex(0, 0)] &&
					m_Elements[GetIndex(0, 1)] == other.m_Elements[GetIndex(0, 1)] &&
					m_Elements[GetIndex(0, 2)] == other.m_Elements[GetIndex(0, 2)] &&
					m_Elements[GetIndex(0, 3)] == other.m_Elements[GetIndex(0, 3)] &&

					m_Elements[GetIndex(1, 0)] == other.m_Elements[GetIndex(1, 0)] &&
					m_Elements[GetIndex(1, 1)] == other.m_Elements[GetIndex(1, 1)] &&
					m_Elements[GetIndex(1, 2)] == other.m_Elements[GetIndex(1, 2)] &&
					m_Elements[GetIndex(1, 3)] == other.m_Elements[GetIndex(1, 3)] &&

					m_Elements[GetIndex(2, 0)] == other.m_Elements[GetIndex(2, 0)] &&
					m_Elements[GetIndex(2, 1)] == other.m_Elements[GetIndex(2, 1)] &&
					m_Elements[GetIndex(2, 2)] == other.m_Elements[GetIndex(2, 2)] &&
					m_Elements[GetIndex(2, 3)] == other.m_Elements[GetIndex(2, 3)] &&

					m_Elements[GetIndex(3, 0)] == other.m_Elements[GetIndex(3, 0)] &&
					m_Elements[GetIndex(3, 1)] == other.m_Elements[GetIndex(3, 1)] &&
					m_Elements[GetIndex(3, 2)] == other.m_Elements[GetIndex(3, 2)] &&
					m_Elements[GetIndex(3, 3)] == other.m_Elements[GetIndex(3, 3)]);
			}

			///////
			// Useful Matrices
			///////

			static inline constexpr Mat4x4<T> Translate(const TVec3<T>& translation)
			{
				Mat4x4 result;

				result.m_Elements[GetIndex(3, 0)] = translation.x;
				result.m_Elements[GetIndex(3, 1)] = translation.y;
				result.m_Elements[GetIndex(3, 2)] = translation.z;
				return result;
			}

			static inline constexpr Mat4x4<T> Scale(const TVec3<T>& scale)
			{
				Mat4x4 result;

				result.m_Elements[GetIndex(0, 0)] = scale.x;
				result.m_Elements[GetIndex(1, 1)] = scale.y;
				result.m_Elements[GetIndex(2, 2)] = scale.z;
				return result;
			}

			// Recommended to normalize axis
			static inline constexpr Mat4x4<T> Rotate(const T& degrees, const TVec3<T>& axis)
			{
				Mat4x4 result;

				const float r = ToRadians(degrees);

				const float c = cos(r);
				const float s = sin(r);

				const TVec3<T> temp((T(1) - c) * axis);

				result.m_Elements[GetIndex(0, 0)] = c + axis.x * temp.x;
				result.m_Elements[GetIndex(0, 1)] = s * axis.z + temp.x * axis.y;
				result.m_Elements[GetIndex(0, 2)] = -s * axis.y + temp.x * axis.z ;
				
				result.m_Elements[GetIndex(1, 0)] = -s * axis.z + temp.y * axis.x;
				result.m_Elements[GetIndex(1, 1)] = c + axis.y * temp.y;
				result.m_Elements[GetIndex(1, 2)] = s * axis.x + temp.y * axis.z;

				result.m_Elements[GetIndex(2, 0)] = s * axis.y + temp.z * axis.x;
				result.m_Elements[GetIndex(2, 1)] = -s * axis.x + temp.z * axis.y;
				result.m_Elements[GetIndex(2, 2)] = c + axis.z * temp.z;

				return result;
			}


			static inline constexpr Mat4x4<T> Orthographic(float left, float right, float bottom, float top, float zNear, float zFar)
			{

				/*	__												__
					| 2 / (r-l), 0,			0,			-(r+l)/(r-l) |
					| 0,		 2 / (t-b), 0,			-(t+b)/(t-b) |
					| 0,		 0,			-2 / (f-n), -(f+n)/(f-n) |
					| 0,		 0,			0,			1			 |
					¯¯												¯¯
				*/

				// Column major is the reason for the rotated and flipped code

				return Mat4x4(
					2 / (right - left), 0, 0, 0, 
					0, 2 / (top - bottom), 0, 0, 
					0, 0, -2 / (zFar - zNear), 0,
					-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(zFar + zNear) / (zFar - zNear), 1);
			}

			
			// Use degrees
			static inline constexpr Mat4x4<T> Perspective(float fov, float aspectratio, float zNear, float zFar)
			{

				const float tanHalfFov = tan(ToRadians(fov) / 2);

				/*	__														   __
					| s,		 0,			0,					        0		|
					| 0,		 s,			0,					        0		|
					| 0,		 0,			-f / (f - n),		-1		|
					| 0,		 0,			-f * n / (f - n), 0		|
					¯¯														   ¯¯
				*/

				// Column major is the reason for the rotated and flipped code


				return Mat4x4(
					1 / (aspectratio * tanHalfFov), 0, 0, 0,
					0, 1 / tanHalfFov, 0, 0,
					0, 0, -(zFar + zNear) / (zFar - zNear), -1,
					0, 0, -(2 * zFar*zNear) / (zFar - zNear), 0);
			}

			static inline const Mat4x4 LookAt(const Vec3& eye, const Vec3& to)
			{
				Mat4x4 result;

				static const Vec3 up(0.0f, 1.0f, 0.0f);

				const Vec3 f = Normalize(to - eye);
				const Vec3 r = Normalize(Cross(f, up));
				const Vec3 u = Cross(r, f);


				result.m_Elements[GetIndex(0, 0)] = r.x;
				result.m_Elements[GetIndex(1, 0)] = r.y;
				result.m_Elements[GetIndex(2, 0)] = r.z;
				result.m_Elements[GetIndex(0, 1)] = u.x;
				result.m_Elements[GetIndex(1, 1)] = u.y;
				result.m_Elements[GetIndex(2, 1)] = u.z;
				result.m_Elements[GetIndex(0, 2)] = -f.x;
				result.m_Elements[GetIndex(1, 2)] = -f.y;
				result.m_Elements[GetIndex(2, 2)] = -f.z;
				result.m_Elements[GetIndex(3, 0)] = -eye.Dot(r);
				result.m_Elements[GetIndex(3, 1)] = -eye.Dot(u);
				result.m_Elements[GetIndex(3, 2)] = eye.Dot(f);
				return result;
			}

			static inline const Mat4x4 LookAt(const Vec3& eye, const Vec3& to, const Vec3& up)
			{
				Mat4 result;

				const Vec3 f = Normalize(to - eye);
				const Vec3 r = Normalize(Cross(f, up));
				const Vec3 u = Cross(r, f);

				result.m_Elements[GetIndex(0, 0)] = r.x;
				result.m_Elements[GetIndex(1, 0)] = r.y;
				result.m_Elements[GetIndex(2, 0)] = r.z;
				result.m_Elements[GetIndex(0, 1)] = u.x;
				result.m_Elements[GetIndex(1, 1)] = u.y;
				result.m_Elements[GetIndex(2, 1)] = u.z;
				result.m_Elements[GetIndex(0, 2)] = -f.x;
				result.m_Elements[GetIndex(1, 2)] = -f.y;
				result.m_Elements[GetIndex(2, 2)] = -f.z;
				result.m_Elements[GetIndex(3, 0)] = -eye.Dot(r);
				result.m_Elements[GetIndex(3, 1)] = -eye.Dot(u);
				result.m_Elements[GetIndex(3, 2)] = eye.Dot(f);
				return result;
			}

		private:
			static constexpr inline int GetIndex(int column, int row) { return (column * 4) + row; }
		};
		typedef Mat4x4<float> Mat4;
	}
}
