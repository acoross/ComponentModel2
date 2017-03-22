#include "./NetworkWorker.h"
#include <MSWSock.h>

#include "exception.h"

namespace scl
{
	std::atomic<bool> NetworkWorker::_init = false;

	NetworkWorker::NetworkWorker()
	{
		bool exp = false;
		if (_init.compare_exchange_strong(exp, true))
		{
			WSADATA wsaData;
			if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			{
				Exception::RaiseException();
			}
		}
	}

	NetworkWorker::~NetworkWorker()
	{
		bool exp = true;
		if (_init.compare_exchange_strong(exp, false))
		{
			::WSACleanup();
		}
	}

	void NetworkWorker::Run()
	{
		for (auto& l : _listeners)
		{
			l->ProcessAccept();
		}
	}

	void NetworkWorker::ProcessAccept()
	{

	}

	void NetworkWorker::AddListener(Sp<class IListener> listener)
	{
		_listeners.push_back(listener);
	}

	void NetworkWorker::IListener::AddListener(Sp<NetworkWorker> worker, Sp<IListener> listener)
	{
		worker->AddListener(listener);
	}
}