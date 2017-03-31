#pragma once

#include "scl/Types.h"
#include "scl/MathLib.h"

namespace scl
{
	class Transform
	{
	public:
		Vector3f _position{ 0, 0, 0 };
		float _yaw{ 0 };
	};
}