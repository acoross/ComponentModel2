#pragma once

#include <string>
#include <cstdint>

#include "Shared.message.h"

namespace SCProtocol
{
	enum class PacketType : uint32_t
	{
		ResponseLogin, 
		NotiMove, 
		NotiEnterZone, 
		NotiLeaveZone, 
		MAX
	};
	
	struct ResponseLogin
	{
		const static uint32_t PacketId = (uint32_t)PacketType::ResponseLogin;
		
		ResponseLogin()
		{
		}
		
		ResponseLogin(bool inok, uint64_t ingameObjectId)
			: ok(inok)
			, gameObjectId(ingameObjectId)
		{
		}
		
		bool ok;
		uint64_t gameObjectId;
		
		template <class Ar>
		void serialize(Ar& ar)
		{
			ar(ok, gameObjectId);
		}
	};
	
	struct NotiMove
	{
		const static uint32_t PacketId = (uint32_t)PacketType::NotiMove;
		
		NotiMove()
		{
		}
		
		NotiMove(uint64_t ingameObjectId, Shared::NetVector inposition, Shared::NetVector invelocity)
			: gameObjectId(ingameObjectId)
			, position(inposition)
			, velocity(invelocity)
		{
		}
		
		uint64_t gameObjectId;
		Shared::NetVector position;
		Shared::NetVector velocity;
		
		template <class Ar>
		void serialize(Ar& ar)
		{
			ar(gameObjectId, position, velocity);
		}
	};
	
	struct NotiEnterZone
	{
		const static uint32_t PacketId = (uint32_t)PacketType::NotiEnterZone;
		
		NotiEnterZone()
		{
		}
		
		NotiEnterZone(uint64_t ingameObjectId, Shared::NetVector inposition, Shared::NetVector invelocity)
			: gameObjectId(ingameObjectId)
			, position(inposition)
			, velocity(invelocity)
		{
		}
		
		uint64_t gameObjectId;
		Shared::NetVector position;
		Shared::NetVector velocity;
		
		template <class Ar>
		void serialize(Ar& ar)
		{
			ar(gameObjectId, position, velocity);
		}
	};
	
	struct NotiLeaveZone
	{
		const static uint32_t PacketId = (uint32_t)PacketType::NotiLeaveZone;
		
		NotiLeaveZone()
		{
		}
		
		NotiLeaveZone(uint64_t ingameObjectId)
			: gameObjectId(ingameObjectId)
		{
		}
		
		uint64_t gameObjectId;
		
		template <class Ar>
		void serialize(Ar& ar)
		{
			ar(gameObjectId);
		}
	};
	
}
