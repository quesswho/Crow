#pragma once
#include "Vec2.h"
#include "../Matrix/Mat3.h"

namespace Crow {
	namespace Math {

		template<typename T>
		struct Mat3x3;

		template<typename T = float>
		struct TVec3
		{
			TVec3()
				: x((T)0), y((T)0), z((T)0)
			{}

			TVec3(T f)
				: x(f), y(f), z(f)
			{}

			TVec3(const TVec2<T>& vec)
				: x(vec.x), y(vec.y), z((T)0)
			{}

			TVec3(const TVec2<T>& vec, T value)
				: x(vec.x), y(vec.y), z(value)
			{}

			TVec3(T first, T second, T third)
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
				this->z = (T)-1;
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
			inline TVec3 Normalize() const
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
			inline constexpr TVec3 Limit(int limit) const
			{
				return this->Normalize() * limit;
			}

			// Limit this magninute with int
			inline void LimitVector(int limit)
			{
				*this = this->Normalize() * limit;
			}

			// Limit magninute with double
			inline TVec3 Limit(double limit) const
			{
				return this->Normalize() * limit;
			}

			// Limit this magninute with double
			inline void LimitVector(double limit)
			{
				*this = this->Normalize() * limit;
			}

			// Limit magninute with float
			inline TVec3 Limit(float limit) const
			{
				return this->Normalize() * limit;
			}

			// Limit this magninute with float
			inline void LimitVector(float limit)
			{
				*this = this->Normalize() * limit;
			}

			// Get distance between this vector and other vector
			inline float Distance(const TVec3& other) const
			{
				return sqrt((this->x - other->x) * (this->x - other->x) +
					(this->y - other->y) * (this->y - other->y) + 
					(this->z - other->z) * (this->z - other->z))
			}

			// Dot product 
			inline float Dot(const TVec3& other) const
			{
				return this->x * other.x + this->y * other.y + this->z * other.z;
			}

			// Dot product 
			inline float Dot(float x, float y, float z) const
			{
				return this->x * x + this->y * y + this->z * z;
			}


			// Cross product
			inline TVec3 Cross(const TVec3& other) const
			{
				return TVec3(this->y * other.z - this->z * other.y, this->z * other.x - this->x * other.z, this->x * other.y - this->y * other.x);
			}

			// Assignment //

			const TVec3& operator=(const int other)
			{
				this->x = other;
				this->y = other;
				this->z = other;
				return *this;
			}

			const TVec3& operator=(const double other)
			{
				this->x = other;
				this->y = other;
				this->z = other;
				return *this;
			}

			const TVec3& operator=(const float other)
			{
				this->x = other;
				this->y = other;
				this->z = other;
				return *this;
			}

			const TVec3& operator=(const TVec3& other)
			{
				this->x = other.x;
				this->y = other.y;
				this->z = other.z;
				return *this;
			}

			// Addition //


			constexpr TVec3& operator+=(const int other)
			{
				this->x += other;
				this->y += other;
				this->z += other;
				return *this;
			}

			friend TVec3 operator+(TVec3 left, const int right) { return left += right; }


			constexpr TVec3& operator+=(const double other)
			{
				this->x += other;
				this->y += other;
				this->z += other;
				return *this;
			}

			friend TVec3 operator+(TVec3 left, const double right) { return left += right; }


			constexpr TVec3& operator+=(const float other)
			{
				this->x += other;
				this->y += other;
				this->z += other;
				return *this;
			}

			friend TVec3 operator+(TVec3 left, const float right) { return left += right; }


			constexpr TVec3& operator+=(const TVec3& other)
			{
				this->x += other.x;
				this->y += other.y;
				this->z += other.z;
				return *this;
			}

			friend TVec3 operator+(TVec3 left, const TVec3& right) { return left += right; }

			// Subtraction //

			constexpr TVec3& operator-=(const int other)
			{
				this->x -= other;
				this->y -= other;
				this->z -= other;
				return *this;
			}

			friend TVec3 operator-(TVec3 left, const int right) { return left -= right; }


			constexpr TVec3& operator-=(const double other)
			{
				this->x -= other;
				this->y -= other;
				this->z -= other;
				return *this;
			}

			friend TVec3 operator-(TVec3 left, const double right) { return left -= right; }


			constexpr TVec3& operator-=(const float other)
			{
				this->x -= other;
				this->y -= other;
				this->z -= other;
				return *this;
			}

			friend TVec3 operator-(TVec3 left, const float right) { return left -= right; }


			constexpr TVec3& operator-=(const TVec3& other)
			{
				this->x -= other.x;
				this->y -= other.y;
				this->z -= other.z;
				return *this;
			}

			friend TVec3 operator-(TVec3 left, const TVec3& right) { return left -= right; }

			// Multiplication //

			constexpr TVec3& operator*=(const int other)
			{
				this->x *= other;
				this->y *= other;
				this->z *= other;
				return *this;
			}

			friend TVec3 operator*(TVec3 left, const int right) { return left *= right; }


			constexpr TVec3& operator*(const double other)
			{
				this->x *= other;
				this->y *= other;
				this->z *= other;
				return *this;
			}

			friend TVec3 operator*=(TVec3 left, const double right) { return left * right; }


			const TVec3& operator*=(const float other)
			{
				this->x *= other;
				this->y *= other;
				this->z *= other;
				return *this;
			}

			friend TVec3 operator*(TVec3 left, const float right) { return left *= right; }


			constexpr TVec3& operator*=(const TVec3& other)
			{
				this->x *= other.x;
				this->y *= other.y;
				this->z *= other.z;
				return *this;
			}

			friend TVec3 operator*(TVec3 left, const TVec3& right) { return left *= right; }

			// Division //

			constexpr TVec3& operator/=(const int other)
			{
				this->x /= other;
				this->y /= other;
				this->z /= other;
				return *this;
			}

			friend TVec3 operator/(TVec3 left, const int right) { return left /= right; }


			constexpr TVec3& operator/=(const double other)
			{
				this->x /= other;
				this->y /= other;
				this->z /= other;
				return *this;
			}

			friend TVec3 operator/(TVec3 left, const double right) { return left /= right; }


			constexpr TVec3& operator/=(const float other)
			{
				this->x /= other;
				this->y /= other;
				this->z /= other;
				return *this;
			}

			friend TVec3 operator/(TVec3 left, const float right) { return left /= right; }


			constexpr TVec3& operator/=(const TVec3& other)
			{
				this->x /= other.x;
				this->y /= other.y;
				this->z /= other.z;
				return *this;
			}

			friend TVec3 operator/(TVec3 left, const TVec3& right) { return left /= right; }

			// Modulus //

			constexpr TVec3& operator%=(const int other)
			{
				this->x %= other;
				this->y %= other;
				this->z %= other;
				return *this;
			}

			friend TVec3 operator%(TVec3 left, const int right) { return left %= right; }


			constexpr TVec3& operator%=(const double other)
			{
				this->x %= other;
				this->y %= other;
				this->z %= other;
				return *this;
			}

			friend TVec3 operator%(TVec3 left, const double right) { return left %= right; }


			constexpr TVec3& operator%=(const float other)
			{
				this->x %= other;
				this->y %= other;
				this->z %= other;
				return *this;
			}

			friend TVec3 operator%(TVec3 left, const float right) { return left %= right; }


			constexpr TVec3& operator%=(const TVec3& other)
			{
				this->x %= other.x;
				this->y %= other.y;
				this->z %= other.z;
				return *this;
			}

			friend TVec3 operator%(TVec3 left, const TVec3& right) { return left %= right; }

			// Test //

			const bool operator==(const TVec3& other) const
			{
				return (this->x == other.x && this->y == other.y && this->z == other->z);
			}

			const bool operator!=(const TVec3& other) const
			{
				return !(this->x == other.x && this->y == other.y && this->z == other.z);
			}

			const bool operator<(const TVec3& other) const
			{
				return (this->x < other.x && this.y < other.y && this->z < other.z);
			}
			const bool operator>(const TVec3& other) const
			{
				return (this->x > other.x&& this.y > other.y && this->z > other.z);
			}

			const bool operator<=(const TVec3& other) const
			{
				return (this->x <= other.x && this.y <= other.y && this.z <= other.z);
			}
			const bool operator>=(const TVec3& other) const
			{
				return (this->x >= other.x && this.y >= other.y && this.z >= other.z);
			}
		};
		typedef TVec3<float> Vec3;
	}
}
