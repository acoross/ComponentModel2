#pragma once

#include <functional>

#include "scl/IO/Network/NetworkInitializer.h"
#include "scl/IO/Network/NetworkWorker.h"
#include "scl/IO/Network/Session.h"

namespace scl
{
	class Listener
		: public NetworkWorker::IListener
		, public std::enable_shared_from_this<Listener>
	{
	public:
		using AcceptHandlerType = std::function<void(Sp<Session>)>;

		Listener(Sp<NetworkWorker> networkWorker);
		void Listen(std::string address, unsigned short port, AcceptHandlerType acceptor);

	private:
		// IListener을(를) 통해 상속됨
		virtual void Accept() override;
		virtual void ProcessAccept() override;

	private:
		Wp<NetworkWorker> _networkWorker;

		SOCKET _listenSocket{ INVALID_SOCKET };
		AcceptHandlerType _acceptor;

		char _buf[1024]{ 0, };
		DWORD _dwRecv{ 0 };
		SOCKET _clientsock{ INVALID_SOCKET };
		OVERLAPPED _overlapped{ 0, };
		WSAEVENT _hAcceptEvent{ WSA_INVALID_EVENT };
	};
}