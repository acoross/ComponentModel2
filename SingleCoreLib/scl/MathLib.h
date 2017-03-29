#pragma once

#include <type_traits>

namespace scl
{
	template <class T>
	class Vector3
	{
	public:
		static_assert(std::is_arithmetic<T>::value, "Vector3<T> should have arithmetic template Arg T");
	
		T X;
		T Y;
		T Z;
	};
}