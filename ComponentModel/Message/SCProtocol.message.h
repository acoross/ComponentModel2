#pragma once

#include "Types.h"
using namespace scl;

#include "Shared.shared.h"

namespace SCProtocol
{
	namespace Noti
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
			
			ResponseLogin(bool ok, uint64 gameObjectId)
				: ok(ok)
				, gameObjectId(gameObjectId)
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
			
			NotiMove(uint64 gameObjectId, Shared::FVector position)
				: gameObjectId(gameObjectId)
				, position(position)
			{
			}
			
			uint64 gameObjectId;
			Shared::FVector position;
			
			template <class Ar>
			void serialize(Ar& ar)
			{
				ar(gameObjectId, position);
			}
		};
		
		struct NotiEnterZone
		{
			const static uint32 PacketId = (uint32)PacketType::NotiEnterZone;
			
			NotiEnterZone()
			{
			}
			
			NotiEnterZone(uint64 gameObjectId, Shared::FVector position, Shared::FVector velocity)
				: gameObjectId(gameObjectId)
				, position(position)
				, velocity(velocity)
			{
			}
			
			uint64 gameObjectId;
			Shared::FVector position;
			Shared::FVector velocity;
			
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
			
			NotiLeaveZone(uint64 gameObjectId)
				: gameObjectId(gameObjectId)
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
}
