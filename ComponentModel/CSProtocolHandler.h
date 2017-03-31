#pragma once

#include "scl/IO/Network/CerealPacket.h"
#include "scl/IO/Network/CerealPacketHandlerTable.h"

#include "Message/CSProtocol.message.h"
#include "Message/CSProtocol.stub.h"

using namespace scl;
using namespace CSProtocol;

class CSProtocolHandler : public CSProtocol::IStub
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

	static std::wstring ToString(Shared::NetVector& vec)
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
	static CerealPacketHandlerTable<CSProtocolHandler> _handler;
};
