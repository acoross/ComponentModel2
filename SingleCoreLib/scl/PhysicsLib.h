#pragma once

#include "scl/MathLib.h"

namespace scl
{
	class Transform
	{
	public:
		Vector3f _position{ 0, 0, 0, 0 };
		Vector3f _orientation{ 0, 0, 0, 1 };	// should be normalized, and should be vector
	};
}