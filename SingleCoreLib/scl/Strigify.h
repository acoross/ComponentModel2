#pragma once

#include <string>
#include <sstream>
#include <scl/MathLib.h>

namespace scl
{
	inline std::string ToString(const Vector3f& vec)
	{
		std::stringstream ss;
		ss << "(" << vec.X << ", " << vec.Y << ", " << vec.Z << ")";
		return ss.str();
	}
}