#pragma once

#include "./NetworkInitializer.h"

namespace scl
{
	class IocpTask : public WSAOVERLAPPED
	{
	public:
		IocpTask() : WSAOVERLAPPED() {}
		virtual void Process(DWORD transferred) = 0;
	};
}