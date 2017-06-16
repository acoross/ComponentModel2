#pragma once

#include <string>
#include <cstdint>

#include "Net.stub.h"

#include "SCProtocol.message.h"

namespace SCProtocol
{
	class IHandler
	{
	public:
		virtual ~IHandler() {}
		virtual void Handle(const ResponseLogin& msg) {}
		virtual void Handle(const NotiMove& msg) {}
		virtual void Handle(const NotiEnterZone& msg) {}
		virtual void Handle(const NotiLeaveZone& msg) {}
	};
}
