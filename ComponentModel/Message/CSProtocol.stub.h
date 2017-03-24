#pragma once

#include "./CSProtocol.h"

namespace CSProtocol
{
	namespace CS
	{
		class IStub
		{
			virtual void Process(RequestLogin&) = 0;
			virtual void Process(Move&) = 0;
		};
	}

	namespace SC
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