#include "SingleCoreLib.h"

#include "Pipeline.h"
#include "Network.h"
#include "CSProtocolHandler.h"
#include "MathLib.h"
#include "GameEngine\GameComponent.h"

using namespace scl;
using namespace CSProtocol::Command;

int main()
{
	auto networkWorker = New<NetworkWorker>();
	auto listener = New<Listener>(networkWorker);
	
	CSProtocolHandler::Initialize();
	
	listener->Listen("0.0.0.0", 17777, [](Sp<Session> session)
	{
		Sp<CSProtocolHandler> handler;
		session->StartRecv([handler](char* buf, ulong size, ulong& processed)
		{
			handler->Handle(buf, size, processed);
		});
	});

	Pipeline pipeline;
	pipeline.Push(networkWorker);

	pipeline.Run();
}