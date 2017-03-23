#pragma once

namespace scl
{
	template <class T, class Siz = int>
	class UniqueId
	{
	public:
		static Siz Generate()
		{
			static Siz uid = 0;
			++uid;
			return uid;
		}
	};
}