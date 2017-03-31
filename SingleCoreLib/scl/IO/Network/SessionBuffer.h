#pragma once

#include "./NetworkInitializer.h"
#include "scl/Types.h"

namespace scl
{
	class SessionRecvBuf
	{
	public:
		const static ulong BUFFSIZ = 1024 * 4;

		WSABUF& GetWSABuffer()
		{
			if (offset >= BUFFSIZ)
			{
				Unwind();
			}

			_wsaBuf.buf = buffer + offset;
			_wsaBuf.len = BUFFSIZ - offset;
			return _wsaBuf;
		}

		void Unwind()
		{
			::memmove(buffer, buffer + start, offset - start);
		}

		ulong UnhandledSize() const
		{
			return offset - start;
		}

		ulong start{ 0, };
		ulong offset{ 0, };
		char buffer[BUFFSIZ]{ 0, };

	private:
		WSABUF _wsaBuf;
	};
}