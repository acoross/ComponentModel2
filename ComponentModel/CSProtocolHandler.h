#pragma once

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

	static std::wstring ToString(Shared::FVector& vec)
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