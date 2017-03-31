#include "scl/IO/Pipeline.h"
#include "scl/IO/Network.h"
#include "scl/MathLib.h"

#include "GameEngine/GameComponent.h"
#include "CSProtocolHandler.h"

using namespace scl;

#include "UnitTests/UnitTest.h"

int main(int argc, char **argv)
{
#ifdef _DEBUG
	::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	if (result > 0)
	{
		throw std::exception();
	}
#endif

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