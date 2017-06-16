#pragma once

#include <string>
#include <cstdint>


#include "Net.message.h"

namespace Net
{
	class IHandler
	{
	public:
		virtual ~IHandler() {}
		virtual void Handle(const Vector3& msg) {}
	};
}
