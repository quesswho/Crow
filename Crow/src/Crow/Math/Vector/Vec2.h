#pragma once

namespace Crow {
	namespace Math {

		template<typename T = float>
		struct Vec2
		{
			Vec2()
				: x((T)0), y((T)0)
			{}

			Vec2(T f)
				: x(f), y(f)
			{}

			Vec2(T first, T second)
				: x(first), y(second)
			{}

			union { T x; T r; T u; };
			union { T y; T g; T v; };

			inline void Zero()
			{
				this->x = (T)0;
				this->y = (T)0;
			}

			inline void One()
			{
				this->x = (T)1;
				this->y = (T)1;
			}

			inline void Left()
			{
				this->x = (T)-1;
				this->y = (T)0;
			}

			inline void Right()
			{
				this->x = (T)1;
				this->y = (T)0;
			}

			inline void Down()
			{
				this->x = (T)0;
				this->y = (T)-1;
			}

			inline void Up()
			{
				this->x = (T)0;
				this->y = (T)1;
			}

			// Vector math //

			// Return the magnitude of the vector
			constexpr float Magnitude() const 
			{
				return sqrt((float)(this->x * this->x + this->y * this->y));
			}

			// Return normalized vector
			inline Vec2 Normalize() const
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
			inline Vec2 Limit(int limit) const
			{
				return this->Normalize() * limit;
			}

			// Limit this magninute with int
			inline void LimitVector(int limit)
			{
				*this = this->Normalize() * limit;
			}

			// Limit magninute with double
			inline Vec2 Limit(double limit) const
			{
				return this->Normalize() * limit;
			}

			// Limit this magninute with double
			inline void LimitVector(double limit)
			{
				*this = this->Normalize() * limit;
			}

			// Limit magninute with float
			inline Vec2 Limit(float limit) const
			{	
				return this->Normalize() * limit;
			}

			// Limit this magninute with float
			inline void LimitVector(float limit)
			{
				*this = this->Normalize() * limit;
			}

			// Get distance between this vector and other vector
			inline float Distance(const Vec2& other) const
			{
				return sqrt((this->x - other->x) * (this->x - other->x) + 
					(this->y - other->y) * (this->y - other->y))
			}

			// Dot product 
			inline float Dot(const Vec2& other) const
			{
				return this->x * other.x + this->y * other.y;
			}

			// Get the angle of the vector in degrees
			inline float Degrees() const
			{
				return ToDegrees(Radians());
			}

			// Get the angle between a vector in degrees
			inline float Degrees(const Vec2& other) const
			{
				return ToDegrees(Radians(other));
			}

			// Get the angle between a vector in radians
			inline float Radians(const Vec2& other) const
			{
				return this->Normalize().Dot(other.Normalize());
			}

			// Get the angle of the vector in radians
			inline float Radians() const
			{
				return atan2((float)this->y, (float)this->x);
			}

			// Assignment //

			const Vec2& operator=(const int other)
			{
				this->x = other;
				this->y = other;
				return *this;
			}

			const Vec2& operator=(const double other)
			{
				this->x = other;
				this->y = other;
				return *this;
			}

			const Vec2& operator=(const float other)
			{
				this->x = other;
				this->y = other;
				return *this;
			}

			const Vec2& operator=(const Vec2& other)
			{
				this->x = other.x;
				this->y = other.y;
				return *this;
			}

			// Addition //


			constexpr Vec2& operator+=(const int other)
			{
				this->x += other;
				this->y += other;
				return *this;
			}

			friend Vec2 operator+(Vec2 left, const int right) { return left += right; }


			constexpr Vec2& operator+=(const double other)
			{
				this->x += other;
				this->y += other;
				return *this;
			}

			friend Vec2 operator+(Vec2 left, const double right) { return left += right; }


			constexpr Vec2& operator+=(const float other)
			{
				this->x += other;
				this->y += other;
				return *this;
			}

			friend Vec2 operator+(Vec2 left, const float right) { return left += right; }


			constexpr Vec2& operator+=(const Vec2& other)
			{
				this->x += other.x;
				this->y += other.y;
				return *this;
			}

			friend Vec2 operator+(Vec2 left, const Vec2& right) { return left += right; }

			// Subtraction //

			constexpr Vec2& operator-=(const int other)
			{
				this->x -= other;
				this->y -= other;
				return *this;
			}

			friend Vec2 operator-(Vec2 left, const int right) { return left -= right; }


			constexpr Vec2& operator-=(const double other)
			{
				this->x -= other;
				this->y -= other;
				return *this;
			}

			friend Vec2 operator-(Vec2 left, const double right) { return left -= right; }


			constexpr Vec2& operator-=(const float other)
			{
				this->x -= other;
				this->y -= other;
				return *this;
			}

			friend Vec2 operator-(Vec2 left, const float right) { return left -= right; }


			constexpr Vec2& operator-=(const Vec2& other)
			{
				this->x -= other.x;
				this->y -= other.y;
				return *this;
			}

			friend Vec2 operator-(Vec2 left, const Vec2& right) { return left -= right; }

			// Multiplication //

			constexpr Vec2& operator*=(const int other)
			{
				this->x *= other;
				this->y *= other;
				return *this;
			}

			friend Vec2 operator*(Vec2 left, const int right) { return left *= right; }


			constexpr Vec2& operator*=(const double other)
			{
				this->x *= other;
				this->y *= other;
				return *this;
			}

			friend Vec2 operator*(Vec2 left, const double right) { return left *= right; }


			const Vec2& operator*=(const float other)
			{
				this->x *= other;
				this->y *= other;
				return *this;
			}

			friend Vec2 operator*(Vec2 left, const float right) { return left *= right; }


			constexpr Vec2& operator*=(const Vec2& other)
			{
				this->x *= other.x;
				this->y *= other.y;
				return *this;
			}

			friend Vec2 operator*(Vec2 left, const Vec2& right) { return left *= right; }

			// Division //

			constexpr Vec2& operator/=(const int other)
			{
				this->x /= other;
				this->y /= other;
				return *this;
			}

			friend Vec2 operator/(Vec2 left, const int right) { return left /= right; }


			constexpr Vec2& operator/=(const double other)
			{
				this->x /= other;
				this->y /= other;
				return *this;
			}

			friend Vec2 operator/(Vec2 left, const double right) { return left /= right; }


			constexpr Vec2& operator/=(const float other)
			{
				this->x /= other;
				this->y /= other;
				return *this;
			}

			friend Vec2 operator/(Vec2 left, const float right) { return left /= right; }


			constexpr Vec2& operator/=(const Vec2& other)
			{
				this->x /= other.x;
				this->y /= other.y;
				return *this;
			}

			friend Vec2 operator/(Vec2 left, const Vec2& right) { return left /= right; }

			// Modulus //

			constexpr Vec2& operator%=(const int other)
			{
				this->x %= other;
				this->y %= other;
				return *this;
			}

			friend Vec2 operator%(Vec2 left, const int right) { return left %= right; } 


			constexpr Vec2& operator%=(const double other)
			{
				this->x %= other;
				this->y %= other;
				return *this;
			}

			friend Vec2 operator%(Vec2 left, const double right) { return left %= right; }


			constexpr Vec2& operator%=(const float other)
			{
				this->x %= other;
				this->y %= other;
				return *this;
			}

			friend Vec2 operator%(Vec2 left, const float right) { return left %= right; }


			constexpr Vec2& operator%=(const Vec2& other)
			{
				this->x %= other.x;
				this->y %= other.y;
				return *this;
			}

			friend Vec2 operator%(Vec2 left, const Vec2& right) { return left %= right; }

			// Test //

			const bool operator==(const Vec2& other) const
			{
				return (this->x == other.x && this->y == other.y);
			}

			const bool operator!=(const Vec2& other) const
			{
				return !(this->x == other.x && this->y == other.y);
			}

			const bool operator<(const Vec2& other) const
			{
				return (this->x < other.x && this.y < other.y);
			}
			const bool operator>(const Vec2& other) const
			{
				return (this->x > other.x && this.y > other.y);
			}

			const bool operator<=(const Vec2& other) const
			{
				return (this->x <= other.x && this.y <= other.y);
			}
			const bool operator>=(const Vec2& other) const
			{
				return (this->x >= other.x&& this.y >= other.y);
			}
		};
	}
}
