#include "scl/IO/Network/Session.h"

namespace scl
{
	void Session::StartRecv(PacketHandler recvHandler)
	{
		SessionStatus exp = NotStarted;

		if (!_status.compare_exchange_strong(exp, Working))
		{
			Exception::RaiseException();
		}

		_recvHandler = recvHandler;
		Recv();
	}

	void Session::Send(Sp<_PacketBuffer> buf)
	{
		if (_status != Working)
			return;

		_sendQueue.push(buf);

		bool Exp = false;
		if (_sending.compare_exchange_strong(Exp, true))
		{
			SendPendingData();
		}
	}

	void Session::Recv()
	{
		if (_status != Working)
			return;

		auto* recvTask = new IocpTask();
		recvTask->session = shared_from_this();
		recvTask->Type = IocpType::Recv;

		DWORD flags = 0;
		if (::WSARecv(_socket, &_recvBuf.GetWSABuffer(), 1, NULL, &flags, recvTask, NULL) == SOCKET_ERROR)
		{
			auto err = ::WSAGetLastError();
			if (err != WSA_IO_PENDING)
			{
				Exception::RaiseException();
			}
		}
	}

	void Session::ProcessRecv(ulong transferred)
	{
		if (transferred == 0)
		{
			Close();
			return;
		}

		_recvBuf.end += transferred;

		DWORD processed = 0;
		do
		{
			_recvHandler(_recvBuf.buffer + _recvBuf.start, _recvBuf.UnhandledSize(), processed);
			_recvBuf.start += processed;
		} while (_recvBuf.UnhandledSize() > 0);

		Recv();
	}

	static bool FillSendBuffer(SessionSendBuf& buf, Sp<_PacketBuffer>& data)
	{
		if (buf.EmptySize() < (ulong)data->filled)
		{
			buf.Unwind();

			if (buf.EmptySize() < (ulong)data->filled)
			{
				return false;
			}
		}

		::memcpy(buf.buffer + buf.end, data->buf, data->filled);
		buf.end += data->filled;
		return true;
	}

	void Session::SendPendingData()
	{
		enum class BufferStatus
		{
			EMPTY,
			FILLED_AT_LEAST_ONCE,
			FULL,
		};

		BufferStatus status = BufferStatus::EMPTY;

		{	// data
			Sp<_PacketBuffer> data;
			while (!_pendingQueue.empty())
			{
				data = _pendingQueue.front();
				if (FillSendBuffer(_sendBuf, data))
				{
					status = BufferStatus::FILLED_AT_LEAST_ONCE;
					_pendingQueue.pop();
				}
				else if (status == BufferStatus::EMPTY)
				{
					Exception::RaiseNetworkException(0);
				}
				else
				{
					status = BufferStatus::FULL;
					break;
				}
			}

			if (status != BufferStatus::FULL)
			{
				while (_sendQueue.try_pop(data))
				{
					if (FillSendBuffer(_sendBuf, data))
					{
						status = BufferStatus::FILLED_AT_LEAST_ONCE;
					}
					else if (status == BufferStatus::EMPTY)
					{
						Exception::RaiseNetworkException(0);
					}
					else
					{
						status = BufferStatus::FULL;
						_pendingQueue.push(data);
						break;
					}
				}
			}
		} // data

		if (status == BufferStatus::EMPTY)
		{
			_sending.store(false);
			return;
		}

		auto* sendTask = new IocpTask();
		sendTask->session = shared_from_this();
		sendTask->Type = IocpType::Send;

		DWORD flags = 0;
		if (::WSASend(_socket, &_sendBuf.GetWSABuffer(), 1, NULL, flags, sendTask, NULL) == SOCKET_ERROR)
		{
			auto err = ::WSAGetLastError();
			if (err != WSA_IO_PENDING)
			{
				Exception::RaiseException();
			}
		}
	}

	void Session::ProcessSend(ulong transferred)
	{
		_sendBuf.start += transferred;

		SendPendingData();
	}

	void Session::Close()
	{
		SessionStatus exp = Working;
		if (_status.compare_exchange_strong(exp, Closed))
		{
			auto* iocpTask = new IocpTask;
			iocpTask->Type = IocpType::Close;
			iocpTask->session = shared_from_this();

			::PostQueuedCompletionStatus(_iocp, NULL, 0, iocpTask);
		}
	}

	void Session::ProcessClose()
	{
		_status.store(Closed);

		printf("session close\n");
		::closesocket(_socket);
	}
}