#pragma once

#include <string>
#include <cstdint>

#include "Net.stub.h"
#include "SCProtocol.stub.h"

#include "CSProtocol.message.h"

namespace CSProtocol
{
	class IHandler
	{
	public:
		virtual ~IHandler() {}
		virtual void Handle(const RequestLogin& msg) {}
		virtual void Handle(const Move& msg) {}
	};
}
