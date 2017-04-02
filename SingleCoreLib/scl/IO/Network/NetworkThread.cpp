#include "scl/IO/Network/NetworkThread.h"

#include "scl/exception.h"
#include "scl/IO/Network/Session.h"

namespace scl
{
	NetworkThread::NetworkThread()
	{
		_iocp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 1);
		_thread = std::thread([this]()
		{
			IocpThread();
		});
	}

	NetworkThread::~NetworkThread()
	{
		::CloseHandle(_iocp);
	}

	void NetworkThread::Register(Sp<Session> session)
	{
		HANDLE h = ::CreateIoCompletionPort((HANDLE)session->GetRawSocket(), _iocp, 0, 0);
		_connectedSessions.insert(std::make_pair(session->GetSessionId(), session));
		session->SetIOCP(_iocp);
	}

	void NetworkThread::Unregister(Sp<class Session> session)
	{
		_connectedSessions.erase(session->GetSessionId());
	}

	void NetworkThread::ProcessIo()
	{
		IocpTask* task = nullptr;
		while (_tasks.try_pop(task))
		{
			task->Process((DWORD)task->InternalHigh);
		}
	}

	void NetworkThread::Join()
	{
		_thread.join();
	}

	void NetworkThread::IocpThread()
	{
		for (;;)
		{
			DWORD bytesTransferred = 0;
			ULONG_PTR completionKey = 0;
			IocpTask* iocpTask = nullptr;
			BOOL ok = ::GetQueuedCompletionStatus(_iocp, &bytesTransferred, &completionKey, (OVERLAPPED**)&iocpTask, 0);
			if (!ok)
			{
				DWORD error = ::GetLastError();
				if (error == WAIT_TIMEOUT)
				{
					continue;
				}
				else if (error == ERROR_NETNAME_DELETED)
				{
					iocpTask->Type = IocpType::Close;
				}
				else
				{
					Exception::RaiseException();
				}
			}

			if (iocpTask->Type == IocpType::Send)
			{
				iocpTask->Process((DWORD)iocpTask->InternalHigh);
			}
			else if (iocpTask->Type == IocpType::Close)
			{
				Unregister(iocpTask->session);
				iocpTask->Process(0);
			}
			else
			{
				_tasks.push(iocpTask);
			}
		}
	}
}