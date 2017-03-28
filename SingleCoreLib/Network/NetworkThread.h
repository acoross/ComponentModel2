#pragma once

#include <thread>
#include <map>
#include <queue>
#include <concurrent_queue.h>

#include "Pipeline.h"
#include "memory.h"
#include "TemplateHelper.h"
#include "./NetworkInitializer.h"
#include "Types.h"

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