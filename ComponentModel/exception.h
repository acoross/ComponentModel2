#pragma once

#include <exception>

namespace scl
{
	class Exception
	{
	public:
		static void RaiseException()
		{
			throw std::exception();
		}

		static void RaiseNetworkException(int err)
		{	
			throw std::exception("NetworkException", err);
		}
	};
}
