#pragma once

#include <type_traits>
#include <cmath>

namespace scl
{
	template <class T>
	class Vector3
	{
	public:
		static_assert(std::is_arithmetic<T>::value, "Vector3<T> should have arithmetic template Arg T");
		typedef Vector3<T> MyT;

		T X{ 0 };
		T Y{ 0 };
		T Z{ 0 };
		const T W{ 0 };	// 0: point, !0: vector

		Vector3() : X(0), Y(0), Z(0) {}
		Vector3(T x, T y, T z, T w = 0) : X(x), Y(y), Z(z), W(w) {}

		MyT operator * (T v) const
		{
			return MyT(X * v, Y * v, Z * v);
		}

		MyT operator / (T v) const
		{
			if (v == 0)
			{
				throw std::invalid_argument("Vector3 divided by zero");
			}

			return MyT(X / v, Y / v, Z / v);
		}

		MyT operator + (const MyT& v) const
		{
			return MyT(X + v.X, Y + v.Y, Z + v.Z);
		}
		
		MyT operator - (const MyT& v) const
		{
			return MyT(X - v.X, Y - v.Y, Z - v.Z);
		}

		MyT ComponentProduct(const MyT& v) const
		{
			return MyT(X * v.X, Y * v.Y, Z * v.Z);
		}

		// scalar product
		T operator * (const MyT& v) const
		{
			return (X * v.X + Y * v.Y + Z * v.Z);
		}

		// cross product
		MyT operator % (const MyT& v) const
		{
			return MyT(Y * v.Z - Z * v.Y,
				Z * v.X - X * v.Z,
				X * v.Y - Y * v.X);
		}
		
		// return: yaw radian
		T Yaw() const
		{
			return std::atan2(Y, X);
		}
		
		MyT Invert() const
		{
			return MyT(-X, -Y, -Z);
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

		MyT Normalize() const
		{
			auto mag = Magnitude();
			if (mag > 0)
			{
				return (*this) / mag;
			}

			return (*this);
		}
	};

	typedef Vector3<float> Vector3f;
}