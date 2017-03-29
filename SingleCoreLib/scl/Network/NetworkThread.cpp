#include "./NetworkThread.h"

#include "scl/exception.h"
#include "./Session.h"


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
			delete task;
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
			OVERLAPPED* overlapped;
			BOOL ok = ::GetQueuedCompletionStatus(_iocp, &bytesTransferred, &completionKey, &overlapped, 0);
			if (!ok)
			{
				DWORD error = ::GetLastError();
				if (error == WAIT_TIMEOUT)
				{
					continue;
				}
				else
				{
					Exception::RaiseException();
				}
			}

			_tasks.push((IocpTask*)overlapped);
		}
	}
}