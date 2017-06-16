#pragma once

#include <typeinfo>

namespace scl
{
	template <class T>
	size_t TypeId()
	{
		return typeid(T).hash_code();
	}

	// reference type �� value Ÿ����, foo* = new bar(); (bar �� derived from foo) �� ��쿡 ����� bar type �� ã���ִµ�,
	// pointer type �� ��� ���� ������ Ÿ���� foo* Ÿ������ �ν��ع�����.
	// �׷��Ƿ� TypeId �Լ����� ������ Ÿ���� ���� �� ���� �����Ѵ�.
	template <class T, class = std::enable_if_t<!std::is_pointer<T>::value>>
	size_t TypeId(const T& val)
	{
		return typeid(val).hash_code();
	}
}