#pragma once

#include "shared.shared.h"

namespace CSProtocol
{
	namespace Command
	{
		enum class PacketType : uint32
		{
			RequestLogin, 
			Move, 
			MAX
		};
		
		struct RequestLogin
		{
			const static uint32 PacketId = (uint32)PacketType::RequestLogin;
			
			RequestLogin()
			{
			}
			
			RequestLogin(String id, String password)
				: id(id)
				, password(password)
			{
			}
			
			String id;
			String password;
			
			template <class Ar>
			void serialize(Ar& ar)
			{
				ar(id, password);
			}
		};
		
		struct Move
		{
			const static uint32 PacketId = (uint32)PacketType::Move;
			
			Move()
			{
			}
			
			Move(shared::FVector position, shared::FVector velocity)
				: position(position)
				, velocity(velocity)
			{
			}
			
			shared::FVector position;
			shared::FVector velocity;
			
			template <class Ar>
			void serialize(Ar& ar)
			{
				ar(position, velocity);
			}
		};
		
	}
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
			
			NotiMove(uint64 gameObjectId, shared::FVector position)
				: gameObjectId(gameObjectId)
				, position(position)
			{
			}
			
			uint64 gameObjectId;
			shared::FVector position;
			
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
			
			NotiEnterZone(uint64 gameObjectId, shared::FVector position, shared::FVector velocity)
				: gameObjectId(gameObjectId)
				, position(position)
				, velocity(velocity)
			{
			}
			
			uint64 gameObjectId;
			shared::FVector position;
			shared::FVector velocity;
			
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
