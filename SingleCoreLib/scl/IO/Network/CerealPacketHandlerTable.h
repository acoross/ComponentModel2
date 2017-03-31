#pragma once

#include <map>
#include <functional>
#include "./CerealPacket.h"

namespace scl
{
	template <class HandlerT>
	class CerealPacketHandlerTable
	{
	public:
		template <class MessageT>
		void RegisterHandler()
		{
			_table[MessageT::PacketId] = [](HandlerT& handler, PacketReader& reader)
			{
				MessageT msg;
				reader.ReadBody(msg);
				handler.Process(msg);
			};
		}

		void Handle(HandlerT& handler, char* buf, ulong size, ulong & processed)
		{
			PacketReader reader(buf, size);

			uint32 packetLen, msgType;
			if (!reader.ReadHeader(packetLen, msgType))
			{
				return;
			}

			auto msgLen = packetLen - (ulong)sizeof(uint32) * 2;
			_table[msgType](handler, reader);

			processed += packetLen;
		}

	private:
		std::map<uint32, std::function<void(HandlerT&, PacketReader&)>> _table;
	};
}