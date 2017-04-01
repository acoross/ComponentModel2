#pragma once

#pragma once

#include "scl/Types.h"
using namespace scl;

#include "Shared.message.h"
#include "SCProtocol.message.h"

namespace CSProtocol
{
	enum class PacketType : uint32
	{
		RequestLogin, 
		Move, 
		MAX
	};
	
	enum class TestEnum : int32
	{
		Foo, 
		Bar, 
		Max, 
	};
	
	struct RequestLogin
	{
		const static uint32 PacketId = (uint32)PacketType::RequestLogin;
		
		RequestLogin()
		{
		}
		
		RequestLogin(String inid, String inpassword)
			: id(inid)
			, password(inpassword)
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
		
		Move(Shared::NetVector inposition, Shared::NetVector invelocity)
			: position(inposition)
			, velocity(invelocity)
		{
		}
		
		Shared::NetVector position;
		Shared::NetVector velocity;
		
		template <class Ar>
		void serialize(Ar& ar)
		{
			ar(position, velocity);
		}
	};
	
}
