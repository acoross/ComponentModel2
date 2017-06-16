#pragma once

#include <string>
#include <cstdint>


namespace Net
{
	enum class PacketType : uint32_t
	{
		Vector3, 
		MAX
	};
	
	enum class ESex : int32_t
	{
		Male, 
		Female, 
		Max, 
	};
	
	struct Vector3
	{
		const static uint32_t PacketId = (uint32_t)PacketType::Vector3;
		
		Vector3()
		{
		}
		
		Vector3(float inX, float inY, float inZ)
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
