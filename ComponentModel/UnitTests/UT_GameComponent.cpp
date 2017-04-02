#include "gtest/gtest.h"

#include "GameEngine/GameComponent.h"
#include "GameEngine/EventHandlerBinder.h"
#include "GameEngine/GameObjectContainer.h"

#include "UnitTests/Comp1.h"
#include "UnitTests/Comp2.h"

using namespace scl;
using namespace GameEngine;

namespace UnitTest
{
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

	TEST(ComponentBinder, TestBinder)
	{
		auto gameObject = New<GameEngine::GameObject>();
		auto comp1 = New<Component1>();
		gameObject->SetComponent<Component1>(comp1);
	
		auto comp2 = New<Component2>();
		gameObject->SetComponent<Component2>(comp2);

		comp1->val = 10;
		comp2->val = 100.f;

		EXPECT_FLOAT_EQ(comp1->GetComp2Val(), 100.f);
		EXPECT_EQ(comp2->getComp1Val(), 10);
	}
}