#pragma once

#include <memory>

namespace scl
{
	// memory
	template <class T>
	using Sp = std::shared_ptr<T>;

	template <class T, class... Args>
	Sp<T> New(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template <class T>
	using Wp = std::weak_ptr<T>;

	template <class T>
	using Up = std::unique_ptr<T>;
}