#pragma once

namespace scl
{
	template <class T>
	class NoCopy
	{
	public:
		NoCopy() {}
		NoCopy(NoCopy&) = delete;
		NoCopy& operator=(NoCopy&) = delete;
	};
}