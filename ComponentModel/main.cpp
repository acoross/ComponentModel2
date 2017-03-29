#include "scl/Pipeline.h"
#include "scl/Network.h"
#include "scl/MathLib.h"

#include "GameEngine/GameComponent.h"
#include "CSProtocolHandler.h"

using namespace scl;
using namespace CSProtocol::Command;

#include "UnitTest.h"

int main()
{
	::Tests();

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