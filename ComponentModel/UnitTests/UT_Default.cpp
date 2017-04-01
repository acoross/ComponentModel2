#pragma once

#include "gtest/gtest.h"

#include <iostream>
#include <sstream>
#include <strstream>

#include "cereal/cereal.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/vector.hpp"

#include "scl/time.h"
#include "scl/MathLib.h"
#include "scl/EventDispatcher.h"
#include "GameEngine/GameComponent.h"
#include "GameEngine/GameObjectContainer.h"
#include "GameEngine/LazyMotionObject.h"
#include "GameEngine/EventHandlerBinder.h"

#include "Message/SCProtocol.message.h"

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
	//printBytes(buf, writed);

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

TEST(Default, WeakPtr)
{
	Wp<int> wp;
	EXPECT_EQ(wp.lock(), nullptr);
	EXPECT_EQ(wp.expired(), true);
	auto sp = New<int>(1);
	wp = sp;
	EXPECT_EQ(wp.expired(), false);

}