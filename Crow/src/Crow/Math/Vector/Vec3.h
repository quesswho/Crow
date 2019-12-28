#pragma once
#include "Vec2.h"

namespace Crow {
	namespace Math {

		template<typename T = float>
		struct Vec3
		{
			Vec3()
				: x((T)0), y((T)0), z((T)0)
			{}

			Vec3(T f)
				: x(f), y(f), z(f)
			{}

			Vec3(const Vec2<T>& vec)
				: x(vev.x), y(vec.y), z((T)0)
			{}

			Vec3(const Vec2<T>& vec, T value)
				: x(vev.x), y(vec.y), z(value)
			{}

			Vec3(T first, T second, T third)
				: x(first), y(second), z(third)
			{}

			union { T x; T r; T u; };
			union { T y; T g; T v; };
			union { T z; T b; T w; };

			inline void Zero()
			{
				this->x = (T)0;
				this->y = (T)0;
				this->z = (T)0;
			}

			inline void One()
			{
				this->x = (T)1;
				this->y = (T)1;
				this->z = (T)1;
			}

			inline void Forward()
			{
				this->x = (T)0;
				this->y = (T)0;
				this->z = (T)1;
			}

			inline void Backwards()
			{
				this->x = (T)0;
				this->y = (T)0;
				this->z = (T)1;
			}

			inline void Left()
			{
				this->x = (T)-1;
				this->y = (T)0;
				this->z = (T)0;
			}

			inline void Right()
			{
				this->x = (T)1;
				this->y = (T)0;
				this->z = (T)0;
			}

			inline void Down()
			{
				this->x = (T)0;
				this->y = (T)-1;
				this->z = (T)0;
			}

			inline void Up()
			{
				this->x = (T)0;
				this->y = (T)1;
				this->z = (T)0;
			}

			// Vector math //

			// Return the magnitude of the vector
			inline constexpr float Magnitude() const
			{
				return sqrt((float)(this->x * this->x + this->y * this->y + this->z * this->z));
			}

			// Return normalized vector
			inline Vec3 Normalize() const
			{
				float mag = this->Magnitude();
				if (mag > 0)
					return *this * (1 / mag);

				return *this; // Can't normalize a zero vector
			}

			// Normalize this vector
			inline void NormalizeVector()
			{
				float mag = this->Magnitude();
				if (mag > 0)
					*this *= (1 / mag);
			}

			static inline const Vec3 Normalize(const Vec3 vec)
			{
				float mag = vec.Magnitude();
				if (mag > 0)
					return vec * (1.0f / mag);
				return vec;
			}

			// Return a negative normalized vector
			static inline const Vec3 NegativeNormalize(const Vec3 vec)
			{
				float mag = vec.Magnitude();
				if (mag > 0)
					return vec * -(1.0f / mag);
				return vec;
			}

			// Limit magninute with int
			inline constexpr Vec3 Limit(int limit) const
			{
				return this->Normalize() * limit;
			}

			// Limit this magninute with int
			inline void LimitVector(int limit)
			{
				*this = this->Normalize() * limit;
			}

			// Limit magninute with double
			inline Vec3 Limit(double limit) const
			{
				return this->Normalize() * limit;
			}

			// Limit this magninute with double
			inline void LimitVector(double limit)
			{
				*this = this->Normalize() * limit;
			}

			// Limit magninute with float
			inline Vec3 Limit(float limit) const
			{
				return this->Normalize() * limit;
			}

			// Limit this magninute with float
			inline void LimitVector(float limit)
			{
				*this = this->Normalize() * limit;
			}

			// Get distance between this vector and other vector
			inline float Distance(const Vec3& other) const
			{
				return sqrt((this->x - other->x) * (this->x - other->x) +
					(this->y - other->y) * (this->y - other->y) + 
					(this->z - other->z) * (this->z - other->z))
			}

			// Dot product 
			inline float Dot(const Vec3& other) const
			{
				return this->x * other.x + this->y * other.y + this->z * other->z;
			}

			// Dot product 
			static inline float Dot(const Vec3& first, const Vec3& second)
			{
				return first.x * second.x + first.y * second.y + first.z * second.z;
			}

			// Cross product
			inline Vec3 Cross(const Vec3& other) const
			{
				return Vec3(this->y * other.z - this->z * other.y, this->z * other.x - this->x * other.z, this->x * other.y - this->y * other.x);
			}

			// Cross product
			static inline Vec3 Cross(const Vec3& first, const Vec3& second)
			{
				return Vec3(first.y * second.z - first.z * second.y, first.z * second.x - first.x * second.z, first.x * second.y - first.y * second.x);
			}

			// Assignment //

			const Vec3& operator=(const int other)
			{
				this->x = other;
				this->y = other;
				this->z = other;
				return *this;
			}

			const Vec3& operator=(const double other)
			{
				this->x = other;
				this->y = other;
				this->z = other;
				return *this;
			}

			const Vec3& operator=(const float other)
			{
				this->x = other;
				this->y = other;
				this->z = other;
				return *this;
			}

			const Vec3& operator=(const Vec3& other)
			{
				this->x = other.x;
				this->y = other.y;
				this->z = other.z;
				return *this;
			}

			// Addition //


			constexpr Vec3& operator+=(const int other)
			{
				this->x += other;
				this->y += other;
				this->z += other;
				return *this;
			}

			friend Vec3 operator+(Vec3 left, const int right) { return left += right; }


			constexpr Vec3& operator+=(const double other)
			{
				this->x += other;
				this->y += other;
				this->z += other;
				return *this;
			}

			friend Vec3 operator+(Vec3 left, const double right) { return left += right; }


			constexpr Vec3& operator+=(const float other)
			{
				this->x += other;
				this->y += other;
				this->z += other;
				return *this;
			}

			friend Vec3 operator+(Vec3 left, const float right) { return left += right; }


			constexpr Vec3& operator+=(const Vec3& other)
			{
				this->x += other.x;
				this->y += other.y;
				this->z += other.z;
				return *this;
			}

			friend Vec3 operator+(Vec3 left, const Vec3& right) { return left += right; }

			// Subtraction //

			constexpr Vec3& operator-=(const int other)
			{
				this->x -= other;
				this->y -= other;
				this->z -= other;
				return *this;
			}

			friend Vec3 operator-(Vec3 left, const int right) { return left -= right; }


			constexpr Vec3& operator-=(const double other)
			{
				this->x -= other;
				this->y -= other;
				this->z -= other;
				return *this;
			}

			friend Vec3 operator-(Vec3 left, const double right) { return left -= right; }


			constexpr Vec3& operator-=(const float other)
			{
				this->x -= other;
				this->y -= other;
				this->z -= other;
				return *this;
			}

			friend Vec3 operator-(Vec3 left, const float right) { return left -= right; }


			constexpr Vec3& operator-=(const Vec3& other)
			{
				this->x -= other.x;
				this->y -= other.y;
				this->z -= other.z;
				return *this;
			}

			friend Vec3 operator-(Vec3 left, const Vec3& right) { return left -= right; }

			// Multiplication //

			constexpr Vec3& operator*=(const int other)
			{
				this->x *= other;
				this->y *= other;
				this->z *= other;
				return *this;
			}

			friend Vec3 operator*(Vec3 left, const int right) { return left *= right; }


			constexpr Vec3& operator*(const double other)
			{
				this->x *= other;
				this->y *= other;
				this->z *= other;
				return *this;
			}

			friend Vec3 operator*=(Vec3 left, const double right) { return left * right; }


			const Vec3& operator*=(const float other)
			{
				this->x *= other;
				this->y *= other;
				this->z *= other;
				return *this;
			}

			friend Vec3 operator*(Vec3 left, const float right) { return left *= right; }


			constexpr Vec3& operator*=(const Vec3& other)
			{
				this->x *= other.x;
				this->y *= other.y;
				this->z *= other.z;
				return *this;
			}

			friend Vec3 operator*(Vec3 left, const Vec3& right) { return left *= right; }

			// Division //

			constexpr Vec3& operator/=(const int other)
			{
				this->x /= other;
				this->y /= other;
				this->z /= other;
				return *this;
			}

			friend Vec3 operator/(Vec3 left, const int right) { return left /= right; }


			constexpr Vec3& operator/=(const double other)
			{
				this->x /= other;
				this->y /= other;
				this->z /= other;
				return *this;
			}

			friend Vec3 operator/(Vec3 left, const double right) { return left /= right; }


			constexpr Vec3& operator/=(const float other)
			{
				this->x /= other;
				this->y /= other;
				this->z /= other;
				return *this;
			}

			friend Vec3 operator/(Vec3 left, const float right) { return left /= right; }


			constexpr Vec3& operator/=(const Vec3& other)
			{
				this->x /= other.x;
				this->y /= other.y;
				this->z /= other.z;
				return *this;
			}

			friend Vec3 operator/(Vec3 left, const Vec3& right) { return left /= right; }

			// Modulus //

			constexpr Vec3& operator%=(const int other)
			{
				this->x %= other;
				this->y %= other;
				this->z %= other;
				return *this;
			}

			friend Vec3 operator%(Vec3 left, const int right) { return left %= right; }


			constexpr Vec3& operator%=(const double other)
			{
				this->x %= other;
				this->y %= other;
				this->z %= other;
				return *this;
			}

			friend Vec3 operator%(Vec3 left, const double right) { return left %= right; }


			constexpr Vec3& operator%=(const float other)
			{
				this->x %= other;
				this->y %= other;
				this->z %= other;
				return *this;
			}

			friend Vec3 operator%(Vec3 left, const float right) { return left %= right; }


			constexpr Vec3& operator%=(const Vec3& other)
			{
				this->x %= other.x;
				this->y %= other.y;
				this->z %= other.z;
				return *this;
			}

			friend Vec3 operator%(Vec3 left, const Vec3& right) { return left %= right; }

			// Test //

			const bool operator==(const Vec3& other) const
			{
				return (this->x == other.x && this->y == other.y && this->z == other->z);
			}

			const bool operator!=(const Vec3& other) const
			{
				return !(this->x == other.x && this->y == other.y && this->z == other.z);
			}

			const bool operator<(const Vec3& other) const
			{
				return (this->x < other.x && this.y < other.y && this->z < other.z);
			}
			const bool operator>(const Vec3& other) const
			{
				return (this->x > other.x&& this.y > other.y && this->z > other.z);
			}

			const bool operator<=(const Vec3& other) const
			{
				return (this->x <= other.x && this.y <= other.y && this.z <= other.z);
			}
			const bool operator>=(const Vec3& other) const
			{
				return (this->x >= other.x && this.y >= other.y && this.z >= other.z);
			}
		};
	}
}
