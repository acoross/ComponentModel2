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

			int32 packetLen, msgType;
			if (!reader.ReadHeader(packetLen, msgType))
			{
				return;
			}

			auto msgLen = packetLen - (int32)sizeof(int32) * 2;
			_table[msgType](handler, reader);

			processed += packetLen;
		}

	private:
		std::map<int32, std::function<void(HandlerT&, PacketReader&)>> _table;
	};
}