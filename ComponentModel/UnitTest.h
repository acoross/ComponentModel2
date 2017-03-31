#pragma once

#include <iostream>
#include <sstream>
#include "scl/MathLib.h"
#include "scl/EventDispatcher.h"
#include "GameEngine/GameComponent.h"

#include "gtest/gtest.h"

using namespace scl;

inline std::string ToString(const Vector3f& vec)
{
	std::stringstream ss;
	ss << "(" << vec.X << ", " << vec.Y << ", " << vec.Z << ")";
	return ss.str();
}

TEST(Default, TestVectorDiv)
{
	Vector3<float> vec(3, 6, 9);
	EXPECT_EQ(vec / 3, Vector3<float>(1, 2, 3));
	EXPECT_EQ(vec * 4, Vector3f(12, 24, 36));

	Vector3f vec2(5, 5, 5);
	EXPECT_EQ(vec + vec2, Vector3f(8, 11, 14));
	EXPECT_EQ(vec - vec2, Vector3f(-2, 1, 4));
	EXPECT_EQ(vec * vec2, 15 + 30 + 45);
	
	// 외적... 맞겠지 뭐
	//printVec(vec % vec2);

	EXPECT_FLOAT_EQ(vec.SqrMagnitude(), 9 + 36 + 81);
	EXPECT_FLOAT_EQ(vec.Magnitude(), std::sqrtf(9 + 36 + 81));

	EXPECT_EQ(vec.ComponentProduct(vec2), Vector3f(15, 30, 45));

	EXPECT_EQ(vec.Invert(), Vector3f(-3, -6, -9));

	//
	EXPECT_FLOAT_EQ(vec.Normalize().Magnitude(), 1);
	
	//
	EXPECT_FLOAT_EQ(vec.Yaw(), Vector3f(1, 2, 3).Yaw());
}

TEST(Default, TestEventDispatcher)
{
	EventDispatcher e1;
	bool invoked = false;
	e1.RegisterHandler<int>([&invoked](const int& e)
	{
		invoked = true;
	});

	Event<int> ee(1);
	invoked = false;
	e1(ee);
	EXPECT_EQ(invoked, true);

	invoked = false;
	e1.InvokeEvent(ee);
	EXPECT_EQ(invoked, true);

	invoked = false;
	e1.InvokeEvent(Event<int>(2));
	EXPECT_EQ(invoked, true);
	
	invoked = false;
	e1.InvokeEvent(Event<float>(3.f));
	EXPECT_EQ(invoked, false);
}

TEST(Default, TestGameObjectSendMsg)
{
	class TestComponent : public GameEngine::GameComponent
	{
	public:
		void OnBound() override
		{
			if (auto o = _owner.lock())
			{
				o->RegisterMsgHandler<int>([&](const int& v)
				{
					value = 1;
					std::cout << v << std::endl;
				});
				o->RegisterMsgHandler<std::string>([&](const std::string& str)
				{
					value = 2;
				});
			}
		}

		int value = 0;
	};

	auto obj = New<GameEngine::GameObject>();
	auto comp = New<TestComponent>();

	obj->SetComponent<TestComponent>(comp);

	comp->value = 0;
	obj->SendMsg(Event<int>(1));
	EXPECT_EQ(comp->value, 1);

	comp->value = 0;
	obj->SendMsg(1);
	EXPECT_EQ(comp->value, 1);

	comp->value = 0;
	obj->SendMsg<std::string>("fuck");
	EXPECT_EQ(comp->value, 2);
}