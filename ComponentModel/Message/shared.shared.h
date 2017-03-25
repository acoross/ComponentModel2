#pragma once

#include "Types.h"
using namespace scl;

namespace shared
{
	struct FVector
	{
		FVector()
		{
		}
		
		FVector(float X, float Y, float Z)
			: X(X)
			, Y(Y)
			, Z(Z)
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
	
	enum class ESex : int32
	{
		Male, 
		Female, 
		Max, 
	};
	
}
