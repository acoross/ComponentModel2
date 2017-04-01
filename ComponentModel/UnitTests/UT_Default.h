#pragma once

#include <iostream>
#include <sstream>
#include <strstream>

#include "cereal/cereal.hpp"
#include "cereal/archives/binary.hpp"

#include "scl/time.h"
#include "scl/MathLib.h"
#include "scl/EventDispatcher.h"
#include "GameEngine/GameComponent.h"
#include "GameEngine/GameObjectContainer.h"
#include "GameEngine/LazyMotionObject.h"
#include "GameEngine/EventHandlerBinder.h"

#include "Message/SCProtocol.message.h"

#include "gtest/gtest.h"

using namespace scl;

inline std::string ToString(const Vector3f& vec)
{
	std::stringstream ss;
	ss << "(" << vec.X << ", " << vec.Y << ", " << vec.Z << ")";
	return ss.str();
}

inline void printBytes(scl::byte* bytes, int len)
{
	for (int i = 0; i < len; ++i)
	{
		printf("%4d ", bytes[i]);
		if (i % 10 == 9)
		{
			printf("\n");
		}
	}
	printf("\n");
}

TEST(Default, cereal)
{
	scl::byte buf[1024]{ 0, };
	std::ostrstream ss((char*)buf, 1024);

	cereal::BinaryOutputArchive ar(ss);
	ar('c');
	ar((short)50);
	ar((int64)100);
	ar(String(L"shin 일환"));
	ar((char)1);

	ar(std::vector<int>{1, 2, 3, 4, 5});
	ar(SCProtocol::NotiEnterZone(1, { 3,4,5 }, { -1, 0, 1 }));
	ar(123);

	auto writed = ss.tellp();
	printBytes(buf, writed);

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


class SendMsgFixture : public ::testing::Test
{
public:
	class TestComponent : public GameEngine::GameComponent
	{
	public:
		GameEngine::EventHandlerBinder<int> intHandler = { *this, [&](const int& v)
		{
			value = 1;
			std::cout << v << std::endl;
		} };

		GameEngine::EventHandlerBinder<std::string> stringHandler = { *this, [&](const std::string& str)
		{
			value = 2;
		} };

		int value = 0;
	};
};

TEST_F(SendMsgFixture, TestEventDispatcher)
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

TEST_F(SendMsgFixture, TestGameObjectSendMsg)
{
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

TEST_F(SendMsgFixture, GameObjectContainer)
{
	GameEngine::GameObjectContainer container;
	auto obj = New<GameEngine::GameObject>();
	auto comp = New<TestComponent>();

	obj->SetComponent<TestComponent>(comp);

	container.Add(obj);

	comp->value = 0;
	container.BroadcastMsg(Event<int>(1));
	EXPECT_EQ(comp->value, 1);

	comp->value = 0;
	container.BroadcastMsg(1);
	EXPECT_EQ(comp->value, 1);

	comp->value = 0;
	container.BroadcastMsg<std::string>("fuck");
	EXPECT_EQ(comp->value, 2);

	const char* fuck = "fuck";
	comp->value = 0;
	container.BroadcastMsg(fuck);
	EXPECT_EQ(comp->value, 0);
}

TEST(LazyRigidBody, TestLazyMotionObject)
{
	EXPECT_EQ(Math::AdjustAngle(370), 10);
	EXPECT_EQ(Math::AdjustAngle(-240), 120);

	auto tick = scl::GetSystemTickMilli();
	GameEngine::LazyRigidBody body(Vector3f(0, 0, 0), 0, { 0, 0, 0 }, 0, tick);

	auto current = tick + 100;
	EXPECT_EQ(body.Position(current), Vector3f(0, 0, 0));
	EXPECT_EQ(body.Yaw(current), 0);

	body.SetVelocity(Vector3f(1, 2, 3), current);
	body.SetAngVelocity(2, current);

	current += 100;
	EXPECT_EQ(body.Position(current), Vector3f(100, 200, 300));
	EXPECT_EQ(body.Yaw(current), 200);

	EXPECT_EQ(body.Position(current), Vector3f(100, 200, 300));
	EXPECT_EQ(body.Yaw(current), 200);

	current += 100;
	EXPECT_EQ(body.Position(current), Vector3f(200, 400, 600));
	EXPECT_EQ(body.Yaw(current), 40);

	body.SetPosition(Vector3f(1, 2, 3), current);
	EXPECT_EQ(body.Position(current), Vector3f(1, 2, 3));

	current += 200;
	EXPECT_EQ(body.Position(current), Vector3f(201, 402, 603));
}

TEST(Math, DegreeConvert)
{
	EXPECT_FLOAT_EQ(Math::RadianToDegree(Math::PI), 180);
	EXPECT_FLOAT_EQ(Math::RadianToDegree(2 * Math::PI), 360);
	EXPECT_FLOAT_EQ(Math::DegreeToRadian(180), Math::PI);
	EXPECT_FLOAT_EQ(Math::DegreeToRadian(360), 2 * Math::PI);
}