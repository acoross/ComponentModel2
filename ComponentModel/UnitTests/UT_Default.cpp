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
#include "scl/TypeInfo.h"
#include "GameEngine/GameComponent.h"
#include "GameEngine/LazyTransform.h"

#include "Message/SCProtocol.message.h"

using namespace scl;
using namespace GameEngine;

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

TEST(LazyTransform, TestLazyMotionObject)
{
	EXPECT_EQ(Math::AdjustAngle(370), 10);
	EXPECT_EQ(Math::AdjustAngle(-240), 120);

	auto tick = scl::GetSystemTickMilli();
	GameTick::Init();
	GameTick::UpdateTick(tick);
	GameEngine::LazyTransform body(Vector3f(0, 0, 0), 0, { 0, 0, 0 }, 0);

	auto current = tick + 100;
	GameTick::UpdateTick(current);
	EXPECT_EQ(body.Position(), Vector3f(0, 0, 0));
	EXPECT_EQ(body.Yaw(), 0);

	body.SetVelocity(Vector3f(1, 2, 3));
	body.SetAngVelocity(2);

	current += 100;
	GameTick::UpdateTick(current);
	EXPECT_EQ(body.Position(), Vector3f(100, 200, 300));
	EXPECT_EQ(body.Yaw(), 200);

	EXPECT_EQ(body.Position(), Vector3f(100, 200, 300));
	EXPECT_EQ(body.Yaw(), 200);

	current += 100;
	GameTick::UpdateTick(current);
	EXPECT_EQ(body.Position(), Vector3f(200, 400, 600));
	EXPECT_EQ(body.Yaw(), 40);

	body.SetPosition(Vector3f(1, 2, 3));
	EXPECT_EQ(body.Position(), Vector3f(1, 2, 3));

	current += 200;
	GameTick::UpdateTick(current);
	EXPECT_EQ(body.Position(), Vector3f(201, 402, 603));
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

class Foo
{
public:
	virtual ~Foo() {}
};

class Bar : public Foo
{
public:
	virtual ~Bar() {}
};

TEST(Default, RTTI)
{
	Foo f;
	Bar b;
	Foo& rfb = b;
	Foo* pfb = new Bar();

	auto n4 = typeid(Foo).name();
	auto n5 = typeid(Bar).name();
	auto tt = typeid(Foo*).name();
	auto tt1 = typeid(Foo&).name();
	auto tt2 = typeid(Bar*).name();
	auto tt3 = typeid(Bar&).name();

	auto n1 = typeid(f).name();
	auto bdd = typeid(b).name();

	auto nfb = typeid(rfb).name();
	auto dfd = typeid(std::remove_reference_t<decltype(rfb)>).name();

	auto n3 = typeid(pfb).name();
	auto n3d = typeid(*pfb).name();
	auto n2 = typeid(std::remove_reference_t<decltype(pfb)>).name();

	delete pfb;
}

#define PRNT_V(v) printf("val:"#v" %I64d\n", scl::TypeId(##v))
#define PRNT_T(t) printf("type: "#t" %I64d\n", scl::TypeId<##t>())

TEST(Default, MY_RTTI)
{
	Foo f;
	Bar b;
	Foo& rfb = b;
	Foo* pfb = new Bar();

	PRNT_V(f);
	PRNT_V(b);
	PRNT_V(rfb);
	PRNT_V(*pfb);

	PRNT_T(Foo);
	PRNT_T(Bar);
	
}