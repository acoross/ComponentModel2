#include "CSProtocolHandler.h"
using namespace CSProtocol;

scl::CerealPacketHandlerTable<CSProtocolHandler> CSProtocolHandler::_handler;

void CSProtocolHandler::Initialize()
{
	static bool initialized = false;
	if (initialized)
		return;

	initialized = true;

	_handler.RegisterHandler<RequestLogin>();
	_handler.RegisterHandler<Move>();
}
