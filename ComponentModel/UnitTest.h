#pragma once

#include "scl/MathLib.h"

using namespace scl;
using namespace GameEngine;

inline std::string ToString(const Vector3f& vec)
{
	std::stringstream ss;
	ss << "(" << vec.X << ", " << vec.Y << ", " << vec.Z << ")";
	return ss.str();
}

inline void printVec(const Vector3f& vec)
{
	std::cout << ToString(vec).c_str() << std::endl;
}

inline void TestVectorDiv()
{
	Vector3f vec(1, 2, 3);
	printVec(vec / 3);

	//
	printVec(vec * 4);

	// 
	Vector3f vec2(5, 5, 5);
	printVec(vec + vec2);

	//
	printVec(vec - vec2);

	//
	printf("%f\n", vec * vec2);

	//
	printVec(vec % vec2);

	//
	printf("%f\n", vec.Magnitude());
	printf("%f\n", vec.SqrMagnitude());

	//
	printVec(vec.ComponentProduct(vec2));

	//
	printVec(vec.Invert());

	//
	printVec(vec.Normalize());
	printf("%f\n", vec.Normalize().Magnitude());
	
	//
	printf("%f\n", vec.Yaw());
}

inline void Tests()
{
	TestVectorDiv();
}

