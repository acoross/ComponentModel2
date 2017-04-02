#pragma once

#include <string>
#include <cstdint>

#include "Shared.message.h"
#include "SCProtocol.message.h"

namespace CSProtocol
{
	enum class PacketType : uint32_t
	{
		RequestLogin, 
		Move, 
		MAX
	};
	
	enum class TestEnum : int32_t
	{
		Foo, 
		Bar, 
		Max, 
	};
	
	struct RequestLogin
	{
		const static uint32_t PacketId = (uint32_t)PacketType::RequestLogin;
		
		RequestLogin()
		{
		}
		
		RequestLogin(std::wstring inid, std::wstring inpassword)
			: id(inid)
			, password(inpassword)
		{
		}
		
		std::wstring id;
		std::wstring password;
		
		template <class Ar>
		void serialize(Ar& ar)
		{
			ar(id, password);
		}
	};
	
	struct Move
	{
		const static uint32_t PacketId = (uint32_t)PacketType::Move;
		
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
