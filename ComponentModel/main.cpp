#include "Pipeline.h"
#include "Network.h"
#include "CSProtocolHandler.h"
#include "MathLib.h"
#include "GameEngine\Component.h"
#include "GameEngine\GameComponent.h"

using namespace scl;
using namespace CSProtocol::Command;

template <class T>
void testMessageHandlerTable(T& msg)
{
	PacketWriter writer;
	auto buf = writer.Write(msg);

	CSProtocolHandler::Initialize();

	CSProtocolHandler handler;
	ulong processed = 0;
	handler.Handle(buf->buf, buf->filled, processed);
}

class com1 : public GameEngine::GameComponent
{

};

class com2 : public GameEngine::GameComponent
{

};

int main()
{
	auto go = New<GameEngine::GameObject>();
	//go->SetComponent<GameEngine::GameComponent>();
	go->SetComponent<com1>();
	go->SetComponent<com2>();
	
	auto id1 = GameEngine::GameComponent::TypeId<com1>();
	auto id2 = GameEngine::GameComponent::TypeId<com1>();
	auto id3 = GameEngine::GameComponent::TypeId<com2>();
	auto id4 = GameEngine::GameComponent::TypeId<com2>();

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