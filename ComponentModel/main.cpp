#include "Pipeline.h"
#include "Network.h"

using namespace scl;

int main()
{
	auto networkWorker = New<NetworkWorker>();
	auto listener = New<Listener>(networkWorker);
	
	listener->Listen("0.0.0.0", 17777, [](Sp<Session> session)
	{
		session->StartRecv([](const char* buf, ulong size, ulong& processed)
		{
			auto msgLen = *(uint32*)buf;
			if (msgLen < size)
				return;

			auto msgType = *(uint32*)(buf + sizeof(uint32));
			processed += msgLen;
		});
	});

	Pipeline pipeline;
	pipeline.Push(networkWorker);

	pipeline.Run();
}