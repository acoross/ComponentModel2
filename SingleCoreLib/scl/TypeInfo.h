#pragma once

#include <typeinfo>

namespace scl
{
	template <class T>
	size_t TypeId()
	{
		return typeid(T).hash_code();
	}

	// reference type 과 value 타입은, foo* = new bar(); (bar 는 derived from foo) 인 경우에 제대로 bar type 을 찾아주는데,
	// pointer type 의 경우 완전 엉뚱한 타입인 foo* 타입으로 인식해버린다.
	// 그러므로 TypeId 함수에는 포인터 타입을 넣을 수 없게 금지한다.
	template <class T, class = std::enable_if_t<!std::is_pointer<T>::value>>
	size_t TypeId(const T& val)
	{
		return typeid(val).hash_code();
	}
}