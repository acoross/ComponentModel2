#include "scl/IO/Pipeline.h"
#include "scl/IO/Network.h"
#include "scl/MathLib.h"

#include "GameEngine/GameComponent.h"
#include "GameEngine/GameTick.h"

#include "CSProtocolHandler.h"
#include "gtest/gtest.h"

using namespace scl;
using namespace GameEngine;

int main(int argc, char **argv)
{
#ifdef _DEBUG
	::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	if (result > 0)
	{
		throw std::exception();
	}
	printf("\n\n\n");
#endif
	
	GameTick::Init();

	auto networkWorker = New<NetworkWorker>();
	auto listener = New<Listener>(networkWorker);
	
	CSProtocolHandler::Initialize();
	
	listener->Listen("0.0.0.0", 17777, [](Sp<Session> session)
	{
		Sp<CSProtocolHandler> handler = New<CSProtocolHandler>();
		session->StartRecv([handler](char* buf, scl::ulong size, scl::ulong& processed)
		{
			handler->Handle(buf, size, processed);
		});
	});

	printf("Listen start... 17777\n");

	Pipeline pipeline;
	pipeline.Push(networkWorker);
	pipeline.Push(New<GameTickUpdateTask>());

	pipeline.Run();
}