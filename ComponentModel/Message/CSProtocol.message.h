#pragma once

#include "Types.h"
using namespace scl;

#include "Shared.shared.h"

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
			
			Move(Shared::FVector position, Shared::FVector velocity)
				: position(position)
				, velocity(velocity)
			{
			}
			
			Shared::FVector position;
			Shared::FVector velocity;
			
			template <class Ar>
			void serialize(Ar& ar)
			{
				ar(position, velocity);
			}
		};
		
	}
}
