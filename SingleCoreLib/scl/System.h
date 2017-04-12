#pragma once

#include <cstdint>
#include <chrono>

namespace scl
{
	namespace System
	{
		inline int64_t GetTickMilliseconds()
		{
			auto now = std::chrono::system_clock::now();
			return std::chrono::duration_cast<std::chrono::milliseconds>(
				now.time_since_epoch()
				).count();
		}
	}
}