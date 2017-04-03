#pragma once

#include "scl/TypeInfo.h"

namespace devs
{
	/*
	장점: 간결하다. 착각할 여지도 없다.

	단점: message 하나 보낼 때 마다 전체 순회하므로 비효율적이다.
	*/

	class IFLMsg
	{
	public:
		virtual ~IFLMsg() {}
	};

	class FLMsg1 : public IFLMsg
	{
	public:
		virtual ~FLMsg1() {}
	};

	class IFLComponent
	{
	public:
		virtual ~IFLComponent() {}
		
		virtual void Dispatch(const IFLMsg& msg) = 0;
	};

	class FLComponent : public IFLComponent
	{
	public:
		virtual ~FLComponent() {}

		virtual void Dispatch(const IFLMsg& msg) final
		{
			if (const auto* msg1 = dynamic_cast<const FLMsg1*>(&msg))
			{
				//...
			}
			else
			{
				//...
			}
		}
	};
}