#pragma once

#include <chrono>
#include "scl/Types.h"

namespace scl
{
	inline int64 GetSystemTickMilli()
	{
		auto tickms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());

		return tickms.count();
	}
}