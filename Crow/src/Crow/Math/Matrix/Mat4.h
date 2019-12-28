#pragma once
#include "../Vector/Vec4.h"

#include "Crow/Math/Math.h"
namespace Crow {
	namespace Math
	{

		// Column Major 4x4 matrix
		template<typename T = float>
		struct Mat4 {

			Mat4() { Identity(); }

			Mat4(const Vec4<T>& first, const Vec4<T>& second, const Vec4<T>& third, const Vec4<T>& forth)
			{
				m_Elements[GetIndex(0, 0)] = first.x;
				m_Elements[GetIndex(0, 1)] = first.y;
				m_Elements[GetIndex(0, 2)] = first.z;
				m_Elements[GetIndex(0, 3)] = first.z;

				m_Elements[GetIndex(1, 0)] = second.x;
				m_Elements[GetIndex(1, 1)] = second.y;
				m_Elements[GetIndex(1, 2)] = second.z;
				m_Elements[GetIndex(1, 3)] = second.z;

				m_Elements[GetIndex(2, 0)] = third.x;
				m_Elements[GetIndex(2, 1)] = third.y;
				m_Elements[GetIndex(2, 2)] = third.z;
				m_Elements[GetIndex(2, 3)] = third.z;

				m_Elements[GetIndex(3, 0)] = forth.x;
				m_Elements[GetIndex(3, 1)] = forth.y;
				m_Elements[GetIndex(3, 2)] = forth.z;
				m_Elements[GetIndex(3, 3)] = forth.z;
			}

			Mat4(T first, T second, T third, T forth, T fifth, T sixth, T seventh, T eighth, T ninth, T tenth, T eleventh, T twelveth, T thirteenth, T fourteenth, T fifteenth, T sixteenth)
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

			union {
				T m_Elements[16];
				Vec4<T> m_Columns[4];
			};

			// Addition

			constexpr Mat4& operator+=(const float scalar)
			{
				for (int i = 0; i < 16; i++)
				{
					this->m_Elements[i] += scalar
				}
				return *this;
			}

			friend Mat4 operator+(Mat4 left, const float right) { return left += right; }

			constexpr Mat4& operator+=(const Mat4& other)
			{
				for (int i = 0; i < 16; i++)
				{
					this->m_Elements[i] += other.m_Elements[i];
				}
				return *this;
			}

			friend Mat4 operator+(Mat4 left, const Mat4& right) { return left += right; }

			// Subtraction

			constexpr Mat4& operator-=(const float scalar)
			{
				for (int i = 0; i < 16; i++)
				{
					this->m_Elements[i] -= scalar
				}
				return *this;
			}

			friend Mat4 operator-(Mat4 left, const float right) { return left -= right; }

			constexpr Mat4& operator-=(const Mat4& other)
			{
				for (int i = 0; i < 16; i++)
				{
					this->m_Elements[i] -= other.m_Elements[i];
				}
				return *this;
			}

			friend Mat4 operator-(Mat4 left, const Mat4& right) { return left -= right; }

			// Multiplication
			
			constexpr Mat4& operator*=(const float scalar)
			{
				for (int i = 0; i < 16; i++)
					m_Elements[i] *= scalar;

				return *this;
			}

			friend Mat4 operator*(Mat4 left, const float right) { return left *= right; }

			constexpr Mat4& operator*=(const Mat4& other)
			{
				int r = 0;
				for (int i = 0; i < 16; i++)
				{
					r = i % 4;
					m_Elements[i] = other.m_Columns[(int)i / 4].Dot(m_Elements[0 * 4 + r], m_Elements[1 * 4 + r], m_Elements[2 * 4 + r], m_Elements[3 * 4 + r]);

				}
				return *this;
			}

			friend Mat4 operator*(Mat4 left, const Mat4& right) { return left *= right; }


			// Assignment

			const Mat4& operator=(const Mat4& other)
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

			static constexpr Mat4 Translate(const Vec3<T>& translation)
			{
				Mat4 result = Mat4();

				result.m_Elements[GetIndex(3, 0)] = translation.x;
				result.m_Elements[GetIndex(3, 1)] = translation.y;
				result.m_Elements[GetIndex(3, 2)] = translation.z;
				return result;
			}

			static constexpr Mat4 Scale(const Vec3<T>& scale)
			{
				Mat4 result = Mat4();

				result.m_Elements[GetIndex(0, 0)] = scale.x;
				result.m_Elements[GetIndex(1, 1)] = scale.y;
				result.m_Elements[GetIndex(2, 2)] = scale.z;
				return result;
			}

			// Recommended to normalize axis
			static constexpr Mat4 Rotate(const float degrees, const Vec3<T>& axis)
			{

				const float r = ToRadians(degrees);

				const float c = cos(r);
				const float s = sin(r);

				const float cs = 1.0 - c;

				Mat4 result = Mat4();

				result.m_Elements[GetIndex(0, 0)] = c + axis.u * axis.u * cs;
				result.m_Elements[GetIndex(0, 1)] = -axis.w * s + axis.u * axis.v * cs;
				result.m_Elements[GetIndex(0, 2)] = axis.v * s + axis.u * axis.w * cs;
				// result.m_Elements[GetIndex(0, 3)] = 0;
				result.m_Elements[GetIndex(1, 0)] = axis.w * s + axis.v * axis.u * cs;
				result.m_Elements[GetIndex(1, 1)] = c + axis.v * axis.v * cs;
				result.m_Elements[GetIndex(1, 2)] = -axis.u * s + axis.v * axis.w * cs;
				// result.m_Elements[GetIndex(1, 3)] = 0;
				result.m_Elements[GetIndex(2, 0)] = -axis.v * s + axis.w * axis.u * cs;
				result.m_Elements[GetIndex(2, 1)] = axis.u * s + axis.w * axis.v * cs;
				result.m_Elements[GetIndex(2, 2)] = c + axis.w * axis.w * cs;
				/*	
					result.m_Elements[GetIndex(2, 3)] = 0;
					result.m_Elements[GetIndex(3, 0)] = 0;
					result.m_Elements[GetIndex(3, 1)] = 0;
					result.m_Elements[GetIndex(3, 2)] = 0;
					result.m_Elements[GetIndex(3, 3)] = 1;
				*/
				return result;
			}


			static constexpr Mat4 OrthographicMatrix(float left, float right, float bottom, float top, float zNear, float zFar)
			{

				/*	__												__
					| 2 / (r-l), 0,			0,			-(r+l)/(r-l) |
					| 0,		 2 / (t-b), 0,			-(t+b)/(t-b) |
					| 0,		 0,			-2 / (f-n), -(f+n)/(f-n) |
					| 0,		 0,			0,			1			 |
					¯¯												¯¯
				*/

				// Column major is the reason for the rotated and flipped code

				return Mat4(
					2 / (right - left), 0, 0, 0, 
					0, 2 / (top - bottom), 0, 0, 
					0, 0, -2 / (zFar - zNear), 0,
					-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(zFar + zNear) / (zFar - zNear), 1);
			}

			
			// Use degrees
			static constexpr Mat4 PerspectiveMatrix(float fov, float aspectratio, float zNear, float zFar)
			{

				float const tanHalfFov = tan(ToRadians(fov) / 2);

				/*	__														   __
					| s,		 0,			0,					        0		|
					| 0,		 s,			0,					        0		|
					| 0,		 0,			-f / (f - n),		-1		|
					| 0,		 0,			-f * n / (f - n), 0		|
					¯¯														   ¯¯
				*/

				// Column major is the reason for the rotated and flipped code


				return Mat4(
					1 / (aspectratio * tanHalfFov), 0, 0, 0,
					0, 1 / tanHalfFov, 0, 0,
					0, 0, -(zFar + zNear) / (zFar - zNear), -1,
					0, 0, -(2 * zFar*zNear) / (zFar - zNear), 0);
			}

			static const Mat4 LookAt(const Vec3<T>& from, const Vec3<T>& to)
			{
				Mat4<T> result;

				static const Vec3<T> tmp(0, 1, 0);

				Vec3<T> forward = Vec3<float>::Normalize(from - to);
				Vec3<T> right = Vec3<float>::NegativeNormalize(Vec3<float>::Cross(forward, tmp));
				Vec3<T> up = Vec3<float>::Cross(forward, right);


				result.m_Elements[GetIndex(0, 0)] = right.x;
				result.m_Elements[GetIndex(0, 1)] = right.y;
				result.m_Elements[GetIndex(0, 2)] = right.z;
				result.m_Elements[GetIndex(1, 0)] = up.x;
				result.m_Elements[GetIndex(1, 1)] = up.y;
				result.m_Elements[GetIndex(1, 2)] = up.z;
				result.m_Elements[GetIndex(2, 0)] = forward.x;
				result.m_Elements[GetIndex(2, 1)] = forward.y;
				result.m_Elements[GetIndex(2, 2)] = forward.z;
				result.m_Elements[GetIndex(3, 0)] = -from.Dot(right, from);
				result.m_Elements[GetIndex(3, 1)] = -from.Dot(up, from);;
				result.m_Elements[GetIndex(3, 2)] = -from.Dot(forward, from);
				return result;
			}

		private:
			static constexpr inline int GetIndex(int column, int row) { return (column * 4) + row; }
		};
	}
}