#define ASIO_STANDALONE
#include <asio.hpp>

#include <string>
#include <iostream>
#include "message\CSProtocol.message.h"

#include "scl/IO/Network/CerealPacket.h"
#include "scl/IO/Network/CerealPacketHandlerTable.h"

int main()
{
	asio::io_service ios; 

	asio::ip::tcp::socket socket(ios);
	asio::ip::tcp::resolver resolver(ios);
	asio::connect(socket, resolver.resolve({ "127.0.0.1", "17777" }));

	/*scl::PacketWriter writer;
	CSProtocol::RequestLogin loginMsg(L"shin", L"0000");
	auto buf = writer.Write(loginMsg);*/

	try {
		for (;;)
		{
			char buf[1024]{ 0, };
			char recvBuf[1024]{ 0, };

			printf("send: ");
			std::cin.getline(buf, _countof(buf));
			int len = strnlen_s(buf, _countof(buf));
			if (len > 0)
			{
				if (len != socket.send(asio::buffer(buf, len)))
				{
					throw std::exception();
				}

				ios.run();

				if (0 == socket.receive(asio::buffer(recvBuf, _countof(recvBuf))))
				{
					printf("socket closed\n");
					break;
				}

				ios.run();

				printf("%s\n", buf);
			}
		}
	}
	catch (std::exception ex)
	{

	}
	//
	
	ios.run();
}