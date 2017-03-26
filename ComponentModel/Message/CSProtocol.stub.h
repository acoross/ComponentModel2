#pragma once

#include "./csprotocol.message.h"
#include "./SCProtocol.message.h"

namespace CSProtocol
{
	namespace Command
	{
		class IStub
		{
			virtual void Process(RequestLogin&) = 0;
			virtual void Process(Move&) = 0;
		};
	}
}

namespace SCProtocol
{
	namespace Noti
	{
		class IStub
		{
			virtual void Process(ResponseLogin&) = 0;
			virtual void Process(NotiMove&) = 0;
			virtual void Process(NotiEnterZone&) = 0;
			virtual void Process(NotiLeaveZone&) = 0;
		};
	}
}