#include "gtest/gtest.h"

#include "GameEngine/GameComponent.h"
#include "GameEngine/GameObjectContainer.h"
#include "GameEngine/GameComponentBinder.h"
#include "GameEngine/MessageDispatcher.h"

#include "UnitTests/Comp1.h"
#include "UnitTests/Comp2.h"

using namespace scl;
using namespace GameEngine;

namespace UnitTest
{
	TEST(GameComponentBinder, TestBinder)
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

	class MessageDispatchFixture : public ::testing::Test
	{
	protected:
		class Receiver1 : public GameEngine::GameComponent
		{
		public:
			void SendMessage(int msg)
			{
				recvInt = msg;
			}

			void SendMessage(std::string msg)
			{
				recvString = msg;
			}

			int recvInt{ 0 };
			std::string recvString;
		};

		class Receiver2 : public GameEngine::GameComponent
		{
		public:
			void SendMessage(int msg)
			{
				recvInt = msg;
			}

			int recvInt{ 0 };
		};

		class Receiver3 : public GameEngine::GameComponent
		{
		public:
			void SendMessage(int msg)
			{
				recvInt = msg;
			}

			int recvInt{ 0 };
		};
	};

	TEST(MessageDispatchFixture, ReceiverBox)
	{
		GameEngine::MessageReceiverBox<int> recvBox;
		
		
	}

	TEST(MessageDispatch, Dispatcher)
	{

	}

	TEST(MessageDispatch, Dispatch2GameObject)
	{

	}

	TEST(MessageDispatch, Dispatch2Container)
	{

	}
}