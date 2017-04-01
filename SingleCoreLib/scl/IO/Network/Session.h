#pragma once

#include "./NetworkInitializer.h"

#include <functional>
#include <concurrent_queue.h>
#include <atomic>
#include <deque>
#include <queue>

#include "scl/Types.h"
#include "scl/UniqueId.h"
#include "scl/exception.h"
#include "scl/memory.h"

#include "scl/IO/Network/SessionBuffer.h"
#include "scl/IO/Network/CerealPacket.h"

namespace scl
{
	typedef std::function<void(char* buf, ulong size, ulong& processed)> PacketHandler;

	enum class IocpType
	{
		Recv,
		Send,
		Close
	};

	class Session : public std::enable_shared_from_this<Session>
	{
	public:
		enum SessionStatus
		{
			NotStarted,
			Working,
			Closed,
		};

		Session(SOCKET sock)
			: _socket(sock), _sessionId(UniqueId<Session, uint64>::Generate())
		{}

		SOCKET GetRawSocket() const
		{
			return _socket;
		}

		uint64 GetSessionId() const
		{
			return _sessionId;
		}

		void StartRecv(PacketHandler recvHandler);

		void Close();

		void Send(Sp<_PacketBuffer> buf);

		void SetIOCP(HANDLE iocp)
		{
			_iocp = iocp;
		}

	private:
		void Recv();
		void SendPendingData();

		void ProcessRecv(ulong transferred);
		void ProcessSend(ulong transferred);
		void ProcessClose();

		SOCKET _socket{ INVALID_SOCKET };
		const uint64 _sessionId;
		SessionRecvBuf _recvBuf;
		PacketHandler _recvHandler;

		concurrency::concurrent_queue<Sp<_PacketBuffer>> _sendQueue;
		std::queue<Sp<_PacketBuffer>> _pendingQueue;
		SessionSendBuf _sendBuf;
		std::atomic<bool> _sending{ false };

		HANDLE _iocp;
		std::atomic<SessionStatus> _status{ NotStarted };

		friend class IocpTask;
	};

	class IocpTask : public WSAOVERLAPPED
	{
	public:
		IocpTask() : WSAOVERLAPPED() {}
		void Process(DWORD transferred)
		{
			if (transferred == 0)
			{
				session->ProcessClose();
			}
			else if (Type == IocpType::Recv)
			{
				session->ProcessRecv(transferred);
			}
			else if (Type == IocpType::Send)
			{
				session->ProcessSend(transferred);
			}
			else
			{
				session->ProcessClose();
			}

			delete this;
		}

		IocpType Type;
		Sp<Session> session;
	};
}