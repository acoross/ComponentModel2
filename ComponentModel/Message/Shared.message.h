#pragma once

#include "scl/Types.h"
using namespace scl;


namespace Shared
{
	enum class PacketType : uint32
	{
		NetVector, 
		MAX
	};
	
	enum class ESex : int32
	{
		Male, 
		Female, 
		Max, 
	};
	
	struct NetVector
	{
		const static uint32 PacketId = (uint32)PacketType::NetVector;
		
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
