#pragma once

#include "scl/TypeInfo.h"

namespace devs
{
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