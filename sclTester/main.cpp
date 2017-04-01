#include <iostream>

#include "scl.h"
#include "scl/IO/Pipeline.h"
#include "scl/IO/Network.h"

using namespace scl;

int main()
{
	auto networkWorker = New<NetworkWorker>();
	auto listener = New<Listener>(networkWorker);

	listener->Listen("0.0.0.0", 17777, [](Sp<Session> session)
	{
		printf("connected!\n");

		Wp<Session> wS = session;
		session->StartRecv([wS](char* buf, ulong size, ulong& processed)
		{
			if (auto s = wS.lock())
			{
				printf_s("%s\n", buf);
				
				auto sendbuf = New<_PacketBuffer>(200);
				::memcpy(sendbuf->buf, buf, size);
				sendbuf->filled = size;

				s->Send(sendbuf);
			}
			processed += size;
		});
	});

	printf("Listen start... 17777\n");

	Pipeline pipeline;
	pipeline.Push(networkWorker);

	pipeline.Run();

	return 0;
}