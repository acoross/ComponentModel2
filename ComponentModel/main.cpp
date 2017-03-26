#include "Pipeline.h"
#include "Network.h"
#include <strstream>
#include "Message\CSProtocol.message.h"
#include "Message\CSProtocol.stub.h"
#include "Network\CerealPacket.h"
#include "Network\PacketHandlerTable.h"

using namespace scl;
using namespace CSProtocol::Command;

class CSProtocolHandler : public CSProtocol::Command::IStub
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

	static String ToString(Shared::FVector& vec)
	{
		std::wstringstream stream;
		stream << "(" << vec.X << ", " << vec.Y << ", " << vec.Z << ")";
		return stream.str();
	}

	virtual void Process(Move & msg) override
	{
		wprintf(L"%s, %s\n", ToString(msg.position).c_str(), ToString(msg.velocity).c_str());
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
		CSProtocol::Command::RequestLogin msg;
		msg.id = L"shin";
		msg.password = L"0080";

		testMessageHandlerTable(msg);
	}

	{
		CSProtocol::Command::Move msg;
		msg.position = Shared::FVector(1, 2, 3);
		msg.velocity = Shared::FVector(10, 20, 30);

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