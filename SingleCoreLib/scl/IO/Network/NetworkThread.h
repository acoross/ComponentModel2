#pragma once

#include "scl/IO/Network/NetworkInitializer.h"

#include <thread>
#include <map>
#include <queue>
#include <concurrent_queue.h>

#include "scl/memory.h"
#include "scl/TemplateHelper.h"
#include "scl/Types.h"

#include "scl/IO/Pipeline.h"

namespace scl
{
	class NetworkThread : public NoCopy<NetworkThread>
	{
	public:
		NetworkThread();
		~NetworkThread();

		void Register(Sp<class Session> session);
		void Unregister(Sp<class Session> session);
		void ProcessIo();
		void Join();

	private:
		void IocpThread();

		HANDLE _iocp{ INVALID_HANDLE_VALUE };
		std::thread _thread;
		std::map<uint64, Sp<class Session>> _connectedSessions;
		concurrency::concurrent_queue<class IocpTask*> _tasks;
	};
}