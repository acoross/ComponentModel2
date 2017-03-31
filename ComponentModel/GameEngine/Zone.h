#pragma once

#include "GameEngine/GameObjectContainer.h"
using namespace scl;

#include <functional>

namespace GameEngine
{
	// L2 의 InSight, OutOfSight 에 해당하는 이벤트 처리를 어떻게 할 지 고민된다.
	// InSight, OutOfSight 시에는 '무언가의 정보' 를 broadcast 해야한다.
	// 이 때, 어떤 정보를 broadcast 할 지는 내가 '어떤 객체' 인 지에 따라서 다를 것이다.
	// 

	class AppearingMsg
	{
	public:
		uint64 objectId;

		std::function<void()> f = [this]() {
			objectId = 1;
		};
	};

	class DisappearingMsg
	{
	public:
		uint64 objectId;
	};

	class Zone
	{
	public:
		void EnterZone(Sp<GameObject> gameObject)
		{
			AppearingMsg sendMsg{ gameObject->Id() };

			for (auto otherPair : _container.GetAllObjects())
			{
				otherPair.second->SendMsg(sendMsg);
				gameObject->SendMsg(AppearingMsg{ otherPair.second->Id() });
			}

			_container.Add(gameObject);
		}

		void LeaveZone(Sp<GameObject> gameObject)
		{
			_container.Remove(gameObject);

			DisappearingMsg sendMsg{ gameObject->Id() };
			
			for (auto otherPair : _container.GetAllObjects())
			{
				otherPair.second->SendMsg(sendMsg);
				gameObject->SendMsg(DisappearingMsg{ otherPair.second->Id() });
			}
		}

		GameObjectContainer _container;
	};
}