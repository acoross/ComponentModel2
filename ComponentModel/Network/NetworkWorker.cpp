#include "./NetworkWorker.h"
#include "exception.h"
#include "./Session.h"
#include "./NetworkThread.h"

namespace scl
{
	std::atomic<bool> NetworkWorker::_init = false;

	NetworkWorker::NetworkWorker()
		: _networkThread(New<NetworkThread>())
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

	void NetworkWorker::OnSessionEstablished(Sp<Session> session)
	{
		_networkThread->Register(session);
	}

	void NetworkWorker::Run()
	{
		_networkThread->ProcessIo();

		for (auto& l : _listeners)
		{
			l->ProcessAccept();
		}
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