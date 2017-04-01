#include "Session.h"

namespace scl
{
	void Session::StartRecv(PacketHandler recvHandler)
	{
		if (_recvStarted)
		{
			Exception::RaiseException();
		}

		_recvStarted = true;
		_recvHandler = recvHandler;
		Recv();
	}

	inline void Session::Close()
	{
		::closesocket(_socket);
	}

	void Session::Recv()
	{
		auto iocpTask = new class RecvTask();
		iocpTask->session = shared_from_this();

		DWORD flags = 0;
		if (::WSARecv(_socket, &_recvBuf.GetWSABuffer(), 1, NULL, &flags, iocpTask, NULL) == SOCKET_ERROR)
		{
			auto err = ::WSAGetLastError();
			if (err == WSA_IO_PENDING)
			{
				return;
			}

			Exception::RaiseException();
		}
	}

	void Session::ProcessRecv(DWORD transferred)
	{
		if (transferred == 0)
		{
			Close();
			return;
		}

		_recvBuf.offset += transferred;

		DWORD processed = 0;
		do
		{
			_recvHandler(_recvBuf.buffer + _recvBuf.start, _recvBuf.offset - _recvBuf.start, processed);
			_recvBuf.start += processed;
		} while (_recvBuf.UnhandledSize() > 0);

		Recv();
	}

}