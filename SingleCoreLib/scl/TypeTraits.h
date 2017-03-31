#pragma once

#include <type_traits>

namespace scl
{
	template <class TBase, class T>
	using Require = std::enable_if_t<std::is_base_of<TBase, T>::value>;
	
}