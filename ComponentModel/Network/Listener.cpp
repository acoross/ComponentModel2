#include "./Listener.h"
#include <MSWSock.h>
#include "exception.h"

namespace scl
{
	Listener::Listener(Sp<NetworkWorker> networkWorker)
		: _networkWorker(networkWorker)
	{
		_hAcceptEvent = ::WSACreateEvent();
	}

	void Listener::Listen(std::string address, unsigned short port, AcceptHandlerType acceptor)
	{
		if (_acceptor != nullptr || acceptor == nullptr)
		{
			Exception::RaiseException();
		}
		_acceptor = acceptor;

		if (auto nw = _networkWorker.lock())
		{
			AddListener(nw, shared_from_this());
		}
		else
		{
			Exception::RaiseException();
		}

		DWORD dwFlags = WSA_FLAG_OVERLAPPED;
		_listenSocket = ::WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, dwFlags);
		if (_listenSocket == INVALID_SOCKET)
		{
			auto err = ::WSAGetLastError();
			Exception::RaiseNetworkException(err);
		}

		// bind
		SOCKADDR_IN serveraddr;
		::memset(&serveraddr, 0, sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_port = htons(port);

		serveraddr.sin_addr.S_un.S_addr = inet_addr(address.c_str());

		if (SOCKET_ERROR == ::bind(_listenSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr)))
		{
			auto err = ::WSAGetLastError();
			Exception::RaiseNetworkException(err);
		}

		// listen
		if (SOCKET_ERROR == ::listen(_listenSocket, SOMAXCONN))
		{
			auto err = ::WSAGetLastError();
			Exception::RaiseNetworkException(err);
		}

		Accept();
	}

	void Listener::Accept()
	{
		DWORD dwFlags = WSA_FLAG_OVERLAPPED;
		_clientsock = ::WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, dwFlags);

		::ResetEvent(_hAcceptEvent);
		_overlapped.hEvent = _hAcceptEvent;
		::AcceptEx(_listenSocket, _clientsock, _buf, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &_dwRecv, &_overlapped);
	}

	void Listener::ProcessAccept()
	{
		auto result = ::WaitForSingleObject(_hAcceptEvent, 0);
		if (WAIT_OBJECT_0 == result)
		{

			auto session = New<Session>();
			Accept();
		}
		else if (WAIT_FAILED == result)
		{
			Exception::RaiseException();
		}
	}
}