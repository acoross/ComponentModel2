#pragma once

#include "./NetworkInitializer.h"

#include <functional>

#include "scl/Types.h"
#include "scl/UniqueId.h"
#include "scl/Network/SessionBuffer.h"
#include "scl/Network/IocpTask.h"
#include "scl/exception.h"
#include "scl/memory.h"

namespace scl
{
	typedef std::function<void(char* buf, ulong size, ulong& processed)> PacketHandler;

	class Session : public std::enable_shared_from_this<Session>
	{
	public:
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

	private:
		void Recv();
	
		void ProcessRecv(DWORD transferred);

		class RecvTask : public IocpTask
		{
		public:
			virtual void Process(DWORD transferred) override sealed
			{
				session->ProcessRecv(transferred);
			}

			Sp<Session> session;
		};

		SOCKET _socket{ INVALID_SOCKET };
		const uint64 _sessionId;
		SessionRecvBuf _recvBuf;
		PacketHandler _recvHandler;
		bool _recvStarted{ false };
	};
}