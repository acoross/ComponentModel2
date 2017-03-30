#pragma once

#include "scl/MathLib.h"
#include "scl/EventDispatcher.h"

using namespace scl;

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

inline void StaticEventHandlerTest()
{
	EventDispatcher e1;
	e1.RegisterHandler<Event<int>>([](const Event<int>& e)
	{
		std::cout << e.value << std::endl;
	});

	Event<int> ee(1);
	e1(ee);
	e1.InvokeEvent(ee);
	e1.InvokeEvent(Event<int>(2));
	e1.InvokeEvent(Event<float>(3.f));
}

inline void Tests()
{
	TestVectorDiv();
}
