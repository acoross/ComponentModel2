#include "Pipeline.h"
#include "Network.h"
#include "Message\CSProtocol.h"
#include "Message\CSProtocol.stub.h"
#include "Network\CerealPacket.h"
#include "Network\PacketHandlerTable.h"

using namespace scl;
using namespace CSProtocol::CS;

class CSProtocolHandler : public CSProtocol::CS::IStub
{
public:
	static void Initialize()
	{
		static bool initialized = false;
		if (initialized)
			return;

		initialized = true;

		_handler.RegisterHandler<RequestLogin>();
		_handler.RegisterHandler<Move>();
	}

	void Handle(char* buf, ulong size, ulong& processed)
	{
		_handler.Handle(*this, buf, size, processed);
	}

	// IStub을(를) 통해 상속됨
	virtual void Process(RequestLogin & msg) override
	{
		wprintf(L"%s, %s\n", msg.id.c_str(), msg.password.c_str());
	}

	virtual void Process(Move & msg) override
	{
		wprintf(L"%s, %s\n", msg.position.ToString().c_str(), msg.velocity.ToString().c_str());
	}

private:
	static PacketHandlerTable<CSProtocolHandler> _handler;
};

PacketHandlerTable<CSProtocolHandler> CSProtocolHandler::_handler;

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

int main()
{
	{
		CSProtocol::CS::RequestLogin msg;
		msg.id = L"shin";
		msg.password = L"0080";

		testMessageHandlerTable(msg);
	}

	{
		CSProtocol::CS::Move msg;
		msg.position = CSProtocol::FVector(1, 2, 3);
		msg.velocity = CSProtocol::FVector(10, 20, 30);

		testMessageHandlerTable(msg);
	}

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