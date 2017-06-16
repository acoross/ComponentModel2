#pragma once

#include "scl/IO/Network/NetworkInitializer.h"
#include "scl/Types.h"

namespace scl
{
	class SessionRecvBuf
	{
	public:
		const static ulong BUFFSIZ = 1024 * 4;

		WSABUF& GetWSABuffer()
		{
			if (end >= BUFFSIZ)
			{
				Unwind();
			}

			_wsaBuf.buf = buffer + end;
			_wsaBuf.len = BUFFSIZ - end;
			return _wsaBuf;
		}

		void Unwind()
		{
			::memmove(buffer, buffer + start, end - start);
		}

		ulong UnhandledSize() const
		{
			return end - start;
		}

		ulong start{ 0, };
		ulong end{ 0, };
		char buffer[BUFFSIZ]{ 0, };

	private:
		WSABUF _wsaBuf;
	};

	class SessionSendBuf
	{
	public:
		const static ulong BUFFSIZ = 1024 * 4;

		void Unwind()
		{
			::memmove(buffer, buffer + start, end - start);
		}

		WSABUF& GetWSABuffer()
		{
			if (end >= BUFFSIZ)
			{
				Unwind();
			}

			_wsaBuf.buf = buffer + start;
			_wsaBuf.len = end - start;
			return _wsaBuf;
		}

		ulong UnhandledSize() const
		{
			return end - start;
		}

		ulong EmptySize() const
		{
			return BUFFSIZ - end;
		}

		ulong start{ 0, };
		ulong end{ 0, };
		char buffer[BUFFSIZ]{ 0, };

	private:
		WSABUF _wsaBuf;
	};
}