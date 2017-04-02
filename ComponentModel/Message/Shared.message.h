#pragma once

#include <string>
#include <cstdint>


namespace Shared
{
	enum class PacketType : uint32_t
	{
		NetVector, 
		MAX
	};
	
	enum class ESex : int32_t
	{
		Male, 
		Female, 
		Max, 
	};
	
	struct NetVector
	{
		const static uint32_t PacketId = (uint32_t)PacketType::NetVector;
		
		NetVector()
		{
		}
		
		NetVector(float inX, float inY, float inZ)
			: X(inX)
			, Y(inY)
			, Z(inZ)
		{
		}
		
		float X;
		float Y;
		float Z;
		
		template <class Ar>
		void serialize(Ar& ar)
		{
			ar(X, Y, Z);
		}
	};
	
}
