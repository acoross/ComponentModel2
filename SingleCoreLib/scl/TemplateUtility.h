#pragma once

#include <functional>

namespace scl
{
	bool all()
	{
		return true;
	}

	bool all(bool v)
	{
		return v;
	}

	template <class... Args>
	bool all(bool v, Args... args)
	{
		return v && all(args...);
	}

	template <class T, class... Args>
	bool all(std::function<bool(T)> func, Args... args)
	{
		return all(func(args)...);
	}

	bool one()
	{
		return false;
	}

	bool one(bool v)
	{
		return v;
	}

	template <class... Args>
	bool one(bool v, Args... args)
	{
		return v || one(args...);
	}

	template <class T, class... Args>
	bool one(std::function<bool(T)> func, Args... args)
	{
		return one(func(args)...);
	}
}