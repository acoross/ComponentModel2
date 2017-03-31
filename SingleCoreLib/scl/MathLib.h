#pragma once

#pragma push_macro("min")
#undef min

#include <type_traits>
#include <cmath>
#include <math.h>

#pragma push_macro("PI")
#undef PI

namespace scl
{
	namespace Math
	{
		const float PI = (float)3.14159265358979323846;

		template<class T>
		typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
			almost_equal(T x, T y)
		{
			// the machine epsilon has to be scaled to the magnitude of the values used
			// and multiplied by the desired precision in ULPs (units in the last place)
			return std::abs(x - y) < std::numeric_limits<T>::epsilon() * std::abs(x + y) * 1
				// unless the result is subnormal
				|| std::abs(x - y) < std::numeric_limits<T>::min();
		}

		template<class T>
		typename std::enable_if<std::numeric_limits<T>::is_integer, bool>::type
			almost_equal(T x, T y)
		{
			return x == y;
		}

		float AdjustAngle(float degree)
		{
			if (degree >= 0)
			{
				int n = int(degree / 360);
				return degree - n * 360;
			}
			else
			{
				int n = int(degree / -360);
				return degree - (n - 1) * 360;
			}
		}

		float RadianToDegree(float radian)
		{
			return radian / PI * 180;
		}

		float DegreeToRadian(float degree)
		{
			return degree / 180 * PI;
		}

		float Pi()
		{
			return PI;
		}
	}

	template <class T>
	class Vector3
	{
	public:
		static_assert(std::is_arithmetic<T>::value, "Vector3<T> should have arithmetic template Arg T");

		T X{ 0 };
		T Y{ 0 };
		T Z{ 0 };

		Vector3() : X(0), Y(0), Z(0) {}
		Vector3(T x, T y, T z) : X(x), Y(y), Z(z) {}

		Vector3 operator * (T v) const
		{
			return Vector3(X * v, Y * v, Z * v);
		}

		Vector3 operator / (T v) const
		{
			if (v == 0)
			{
				throw std::invalid_argument("Vector3 divided by zero");
			}

			return Vector3(X / v, Y / v, Z / v);
		}

		Vector3 operator + (const Vector3& v) const
		{
			return Vector3(X + v.X, Y + v.Y, Z + v.Z);
		}
		
		Vector3 operator - (const Vector3& v) const
		{
			return Vector3(X - v.X, Y - v.Y, Z - v.Z);
		}

		friend bool operator == (const Vector3& v1, const Vector3& v)
		{
			return Math::almost_equal<T>(v1.X, v.X) && Math::almost_equal<T>(v1.Y, v.Y) && Math::almost_equal<T>(v1.Z, v.Z);
		}

		Vector3 ComponentProduct(const Vector3& v) const
		{
			return Vector3(X * v.X, Y * v.Y, Z * v.Z);
		}

		// scalar product
		T operator * (const Vector3& v) const
		{
			return (X * v.X + Y * v.Y + Z * v.Z);
		}

		// cross product
		Vector3 operator % (const Vector3& v) const
		{
			return Vector3(Y * v.Z - Z * v.Y,
				Z * v.X - X * v.Z,
				X * v.Y - Y * v.X);
		}
		
		// return: yaw degree
		T Yaw() const
		{
			return Math::RadianToDegree(std::atan2(Y, X));
		}
		
		Vector3 Invert() const
		{
			return Vector3(-X, -Y, -Z);
		}

		float SqrMagnitude() const
		{
			return X * X + Y * Y + Z * Z;
		}

		float Magnitude() const
		{
			auto sqMag = SqrMagnitude();
			return std::sqrt(sqMag);
		}

		Vector3 Normalize() const
		{
			auto mag = Magnitude();
			if (mag > 0)
			{
				return (*this) / mag;
			}

			return (*this);
		}

		Vector3 Rotate2d(float degree)
		{
			float rad = Math::DegreeToRadian(degree);
			return Vector3(X * std::cosf(rad) - Y * std::sinf(rad),
				Y * std::cosf(rad) + X * std::sinf(rad),
				Z);
		}
	};

	typedef Vector3<float> Vector3f;
}

#pragma pop_macro("min")

#undef PI
#pragma pop_macro("PI")