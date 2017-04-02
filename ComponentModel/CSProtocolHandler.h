#pragma once

#include "scl/IO/Network/CerealPacket.h"
#include "scl/IO/Network/CerealPacketHandlerTable.h"

#include "Message/CSProtocol.message.h"
#include "Message/CSProtocol.stub.h"

class CSProtocolHandler : public CSProtocol::IStub
{
public:
	static void Initialize();

	void Handle(char* buf, scl::ulong size, scl::ulong& processed)
	{
		_handler.Handle(*this, buf, size, processed);
	}

	// IStub을(를) 통해 상속됨
	virtual void Process(CSProtocol::RequestLogin& msg) override
	{
		wprintf(L"%s, %s\n", msg.id.c_str(), msg.password.c_str());
	}

	static std::wstring ToString(Shared::NetVector& vec)
	{
		std::wstringstream stream;
		stream << "(" << vec.X << ", " << vec.Y << ", " << vec.Z << ")";
		return stream.str();
	}

	virtual void Process(CSProtocol::Move & msg) override
	{
		wprintf(L"%s, %s\n", ToString(msg.position).c_str(), ToString(msg.velocity).c_str());
	}

private:
	static scl::CerealPacketHandlerTable<CSProtocolHandler> _handler;
};
