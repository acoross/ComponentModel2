#pragma once

#include "scl/Types.h"
using namespace scl;

#include "Shared.message.h"

namespace SCProtocol
{
	enum class PacketType : uint32
	{
		ResponseLogin, 
		NotiMove, 
		NotiEnterZone, 
		NotiLeaveZone, 
		MAX
	};
	
	struct ResponseLogin
	{
		const static uint32 PacketId = (uint32)PacketType::ResponseLogin;
		
		ResponseLogin()
		{
		}
		
		ResponseLogin(bool inok, uint64 ingameObjectId)
			: ok(inok)
			, gameObjectId(ingameObjectId)
		{
		}
		
		bool ok;
		uint64 gameObjectId;
		
		template <class Ar>
		void serialize(Ar& ar)
		{
			ar(ok, gameObjectId);
		}
	};
	
	struct NotiMove
	{
		const static uint32 PacketId = (uint32)PacketType::NotiMove;
		
		NotiMove()
		{
		}
		
		NotiMove(uint64 ingameObjectId, Shared::NetVector inposition, Shared::NetVector invelocity)
			: gameObjectId(ingameObjectId)
			, position(inposition)
			, velocity(invelocity)
		{
		}
		
		uint64 gameObjectId;
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
		const static uint32 PacketId = (uint32)PacketType::NotiEnterZone;
		
		NotiEnterZone()
		{
		}
		
		NotiEnterZone(uint64 ingameObjectId, Shared::NetVector inposition, Shared::NetVector invelocity)
			: gameObjectId(ingameObjectId)
			, position(inposition)
			, velocity(invelocity)
		{
		}
		
		uint64 gameObjectId;
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
		const static uint32 PacketId = (uint32)PacketType::NotiLeaveZone;
		
		NotiLeaveZone()
		{
		}
		
		NotiLeaveZone(uint64 ingameObjectId)
			: gameObjectId(ingameObjectId)
		{
		}
		
		uint64 gameObjectId;
		
		template <class Ar>
		void serialize(Ar& ar)
		{
			ar(gameObjectId);
		}
	};
	
}
