#define ASIO_STANDALONE
#include <asio.hpp>

#include <string>
namespace scl
{
	typedef unsigned char byte;
	typedef unsigned int uint;
	typedef unsigned short ushort;
	typedef __int32 int32;
	typedef unsigned __int32 uint32;
	typedef __int64 int64;
	typedef unsigned __int64 uint64;
	typedef unsigned long ulong;
	typedef std::wstring String;
}
using namespace scl;

#include "message\CSProtocol.message.h"

int main()
{
	asio::io_service ios;

	asio::ip::tcp::socket socket(ios);
	asio::ip::tcp::resolver resolver(ios);
	asio::connect(socket, resolver.resolve({ "127.0.0.1", "17777" }));

	char buf[1024]{ 0, };
	*(uint32_t*)(buf) = sizeof(uint32_t) * 2;
	*(uint32_t*)(buf + sizeof(uint32_t)) = 4;

	socket.send(asio::buffer(buf, sizeof(uint32_t) * 2));

	ios.run();
}