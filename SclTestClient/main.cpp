#include <asio.hpp>

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