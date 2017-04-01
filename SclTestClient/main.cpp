#define ASIO_STANDALONE
#include <asio.hpp>

#include <string>
#include "message\CSProtocol.message.h"

#include "scl/IO/Network/CerealPacket.h"
#include "scl/IO/Network/CerealPacketHandlerTable.h"

int main()
{
	asio::io_service ios; 

	asio::ip::tcp::socket socket(ios);
	asio::ip::tcp::resolver resolver(ios);
	asio::connect(socket, resolver.resolve({ "127.0.0.1", "17777" }));

	scl::PacketWriter writer;
	CSProtocol::RequestLogin loginMsg(L"shin", L"0000");
	auto buf = writer.Write(loginMsg);

	socket.send(asio::buffer(buf->buf, buf->filled));

	ios.run();
}