#pragma once
#include "Vec3.h"
#include "../Matrix/Mat4.h"

namespace Crow {
	namespace Math {

		template<typename T>
		struct Mat4;

		template<typename T = float>
		struct Vec4
		{
			Vec4()
				: x((T)0), y((T)0), z((T)0), w((T)0)
			{}

			Vec4(T f)
				: x(f), y(f), z(f), w(f)
			{}

			Vec4(const Vec2<T>& vec)
				: x(vev.x), y(vec.y), z((T)0), w((T)0)
			{}

			Vec4(const Vec2<T>& vec, T value, T second)
				: x(vec.x), y(vec.y), z(value), w(second)
			{}

			Vec4(const Vec3<T>& vec)
				: x(vec.x), y(vec.y), z((T)vec.z), w((T)0)
			{}

			Vec4(const Vec3<T>& vec, T value)
				: x(vec.x), y(vec.y), z(vec.z), w(value)
			{}

			Vec4(T first, T second, T third, T forth)
				: x(first), y(second), z(third), w(forth)
			{}

			union { T x; T r; };
			union { T y; T g; };
			union { T z; T b; };
			union { T w; T a; };

			inline void Zero()
			{
				this->x = (T)0;
				this->y = (T)0;
				this->z = (T)0;
				this->w = (T)0;
			}

			inline void One()
			{
				this->x = (T)1;
				this->y = (T)1;
				this->z = (T)1;
				this->w = (T)1;
			}

			inline void Forward()
			{
				this->x = (T)0;
				this->y = (T)0;
				this->z = (T)1;
				this->w = (T)0;
			}

			inline void Backwards()
			{
				this->x = (T)0;
				this->y = (T)0;
				this->z = (T)1;
				this->w = (T)0;
			}

			inline void Left()
			{
				this->x = (T)-1;
				this->y = (T)0;
				this->z = (T)0;
				this->w = (T)0;
			}

			inline void Right()
			{
				this->x = (T)1;
				this->y = (T)0;
				this->z = (T)0;
				this->w = (T)0;
			}

			inline void Down()
			{
				this->x = (T)0;
				this->y = (T)-1;
				this->z = (T)0;
				this->w = (T)0;
			}

			inline void Up()
			{
				this->x = (T)0;
				this->y = (T)1;
				this->z = (T)0;
				this->w = (T)0;
			}

			inline void Ana()
			{
				this->x = (T)0;
				this->y = (T)0;
				this->z = (T)0;
				this->w = (T)1;
			}

			inline void Kata()
			{
				this->x = (T)0;
				this->y = (T)0;
				this->z = (T)0;
				this->w = (T)-1;
			}

			// Vector math //

			// Return the magnitude of the vector
			inline constexpr float Magnitude() const
			{
				return sqrt((float)(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w));
			}

			// Return normalized vector
			inline Vec4 Normalize() const
			{
				float mag = this->Magnitude();
				if (mag > 0)
					return *this * (1.0f / mag);

				return *this; // Can't normalize a zero vector
			}

			// Normalize this vector
			inline void NormalizeVector()
			{
				float mag = this->Magnitude();
				if (mag > 0)
					*this *= (1.0f / mag);
			}

			// Limit magninute with int
			inline constexpr Vec4 Limit(int limit) const
			{
				return this->Normalize() * limit;
			}

			// Limit this magninute with int
			inline void LimitVector(int limit)
			{
				*this = this->Normalize() * limit;
			}

			// Limit magninute with double
			inline constexpr Vec4 Limit(double limit) const
			{
				return this->Normalize() * limit;
			}

			// Limit this magninute with double
			inline void LimitVector(double limit)
			{
				*this = this->Normalize() * limit;
			}

			// Limit magninute with float
			inline constexpr Vec4 Limit(float limit) const
			{
				return this->Normalize() * limit;
			}

			// Limit this magninute with float
			inline void LimitVector(float limit)
			{
				*this = this->Normalize() * limit;
			}

			// Get distance between this vector and other vector
			inline float Distance(const Vec4& other) const
			{
				return sqrt((this->x - other->x) * (this->x - other->x) +
					(this->y - other->y) * (this->y - other->y) +
					(this->z - other->z) * (this->z - other->z) +
					(this->w - other->w) * (this->w - other->w));
			}

			// Dot product 
			inline float Dot(const Vec4& other) const
			{
				return this->x * other.x + this->y * other.y + this->z * other.z + this->w * other.w;
			}

			inline float Dot(T otherX, T otherY, T otherZ, T otherW) const
			{
				return this->x * otherX + this->y * otherY + this->z * otherZ + this->w * otherW;
			}

			// Assignment //

			const Vec4& operator=(const int other)
			{
				this->x = other;
				this->y = other;
				this->z = other;
				this->w = other;
				return *this;
			}

			const Vec4& operator=(const double other)
			{
				this->x = other;
				this->y = other;
				this->z = other;
				this->w = other;
				return *this;
			}

			const Vec4& operator=(const float other)
			{
				this->x = other;
				this->y = other;
				this->z = other;
				this->w = other;
				return *this;
			}

			const Vec4& operator=(const Vec4& other)
			{
				this->x = other.x;
				this->y = other.y;
				this->z = other.z;
				this->w = other.w;
				return *this;
			}

			// Addition //


			constexpr Vec4& operator+=(const int other)
			{
				this->x += other;
				this->y += other;
				this->z += other;
				this->w += other;
				return *this;
			}

			friend Vec4 operator+(Vec4 left, const int right) { return left += right; }


			constexpr Vec4& operator+=(const double other)
			{
				this->x += other;
				this->y += other;
				this->z += other;
				this->w += other;
				return *this;
			}

			friend Vec4 operator+(Vec4 left, const double right) { return left += right; }


			constexpr Vec4& operator+=(const float other)
			{
				this->x += other;
				this->y += other;
				this->z += other;
				this->w += other;
				return *this;
			}

			friend Vec4 operator+(Vec4 left, const float right) { return left += right; }


			constexpr Vec4& operator+=(const Vec4& other)
			{
				this->x += other.x;
				this->y += other.y;
				this->z += other.z;
				this->w += other.w;
				return *this;
			}

			friend Vec4 operator+(Vec4 left, const Vec4& right) { return left += right; }

			// Subtraction //

			constexpr Vec4& operator-=(const int other)
			{
				this->x -= other;
				this->y -= other;
				this->z -= other;
				this->w -= other;
				return *this;
			}

			friend Vec4 operator-(Vec4 left, const int right) { return left -= right; }


			constexpr Vec4& operator-=(const double other)
			{
				this->x -= other;
				this->y -= other;
				this->z -= other;
				this->w -= other;
				return *this;
			}

			friend Vec4 operator-(Vec4 left, const double right) { return left -= right; }


			constexpr Vec4& operator-=(const float other)
			{
				this->x -= other;
				this->y -= other;
				this->z -= other;
				this->w -= other;
				return *this;
			}

			friend Vec4 operator-(Vec4 left, const float right) { return left -= right; }


			constexpr Vec4& operator-=(const Vec4& other)
			{
				this->x -= other.x;
				this->y -= other.y;
				this->z -= other.z;
				this->w -= other.w;
				return *this;
			}

			friend Vec4 operator-(Vec4 left, const Vec4& right) { return left -= right; }

			// Multiplication //

			constexpr Vec4& operator*=(const int other)
			{
				this->x *= other;
				this->y *= other;
				this->z *= other;
				this->w *= other;
				return *this;
			}

			friend Vec4 operator*(Vec4 left, const int right) { return left *= right; }


			constexpr Vec4& operator*=(const double other)
			{
				this->x *= other;
				this->y *= other;
				this->z *= other;
				this->w *= other;
				return *this;
			}

			friend Vec4 operator*(Vec4 left, const double right) { return left *= right; }


			const Vec4& operator*=(const float other)
			{
				this->x *= other;
				this->y *= other;
				this->z *= other;
				this->w *= other;
				return *this;
			}

			friend Vec4 operator*(Vec4 left, const float right) { return left *= right; }


			constexpr Vec4& operator*=(const Vec4& other)
			{
				this->x *= other.x;
				this->y *= other.y;
				this->z *= other.z;
				this->w *= other.w;
				return *this;
			}

			friend Vec4 operator*(Vec4 left, const Vec4& right) { return left *= right; }

			constexpr Vec4& operator*=(const Mat4<T> other)
			{
				Vec4<T> result;
				result.x = (T)Dot(other.m_Elements[0], other.m_Elements[4], other.m_Elements[8], other.m_Elements[12]);
				result.y = (T)Dot(other.m_Elements[1], other.m_Elements[5], other.m_Elements[9], other.m_Elements[13]);
				result.z = (T)Dot(other.m_Elements[2], other.m_Elements[6], other.m_Elements[10], other.m_Elements[14]);
				result.w = (T)Dot(other.m_Elements[3], other.m_Elements[7], other.m_Elements[11], other.m_Elements[15]);
				*this = result;
				return *this;
			}

			friend Vec4 operator*(Vec4 left, const Mat4<T>& right) { return left *= right; }

			// Division //

			constexpr Vec4& operator/=(const int other)
			{
				this->x /= other;
				this->y /= other;
				this->z /= other;
				this->w /= other;
				return *this;
			}

			friend Vec4 operator/(Vec4 left, const int right) { return left /= right; }


			constexpr Vec4& operator/=(const double other)
			{
				this->x /= other;
				this->y /= other;
				this->z /= other;
				this->w /= other;
				return *this;
			}

			friend Vec4 operator/(Vec4 left, const double right) { return left /= right; }


			constexpr Vec4& operator/=(const float other)
			{
				this->x /= other;
				this->y /= other;
				this->z /= other;
				this->w /= other;
				return *this;
			}

			friend Vec4 operator/(Vec4 left, const float right) { return left /= right; }


			constexpr Vec4& operator/=(const Vec4& other)
			{
				this->x /= other.x;
				this->y /= other.y;
				this->z /= other.z;
				this->w /= other.w;
				return *this;
			}

			friend Vec4 operator/(Vec4 left, const Vec4& right) { return left /= right; }

			// Modulus //

			constexpr Vec4& operator%=(const int other)
			{
				this->x %= other;
				this->y %= other;
				this->z %= other;
				this->w %= other;
				return *this;
			}

			friend Vec4 operator%(Vec4 left, const int right) { return left %= right; }


			constexpr Vec4& operator%=(const double other)
			{
				this->x %= other;
				this->y %= other;
				this->z %= other;
				this->w %= other;
				return *this;
			}

			friend Vec4 operator%(Vec4 left, const double right) { return left %= right; }


			constexpr Vec4& operator%=(const float other)
			{
				this->x %= other;
				this->y %= other;
				this->z %= other;
				this->w %= other;
				return *this;
			}

			friend Vec4 operator%(Vec4 left, const float right) { return left %= right; }


			constexpr Vec4& operator%=(const Vec4& other)
			{
				this->x %= other.x;
				this->y %= other.y;
				this->z %= other.z;
				this->w %= other.w;
				return *this;
			}

			friend Vec4 operator%(Vec4 left, const Vec4& right) { return left %= right; }

			// Test //

			const bool operator==(const Vec4& other) const
			{
				return (this->x == other.x && this->y == other.y && this->z == other->z && this->w == other->w);
			}

			const bool operator!=(const Vec4& other) const
			{
				return !(this->x == other.x && this->y == other.y && this->z == other.z && this->w == other->w);
			}

			const bool operator<(const Vec4& other) const
			{
				return (this->x < other.x && this->y < other.y && this->z < other.z && this->w < other.w);
			}
			const bool operator>(const Vec4& other) const
			{
				return (this->x > other.x && this->y > other.y && this->z > other.z && this->w > other.w);
			}

			const bool operator<=(const Vec4& other) const
			{
				return (this->x <= other.x && this->y <= other.y && this->z <= other.z && this->w <= other.w);
			}
			const bool operator>=(const Vec4& other) const
			{
				return (this->x >= other.x && this->y >= other.y && this->z >= other.z && this->w >= other.w);
			}
		};
	}
}
