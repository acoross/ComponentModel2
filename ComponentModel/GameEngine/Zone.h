#pragma once

#include <functional>

#include "GameEngine/GameObjectContainer.h"

namespace GameEngine
{
	// L2 �� InSight, OutOfSight �� �ش��ϴ� �̺�Ʈ ó���� ��� �� �� ��εȴ�.
	// InSight, OutOfSight �ÿ��� '������ ����' �� broadcast �ؾ��Ѵ�.
	// �� ��, � ������ broadcast �� ���� ���� '� ��ü' �� ���� ���� �ٸ� ���̴�.
	// 

	class AppearingMsg
	{
	public:
		scl::uint64 objectId;

		std::function<void()> f = [this]() {
			objectId = 1;
		};
	};

	class DisappearingMsg
	{
	public:
		scl::uint64 objectId;
	};

	class Zone
	{
	public:
		void EnterZone(scl::Sp<GameObject> gameObject)
		{
			
		}

		void LeaveZone(scl::Sp<GameObject> gameObject)
		{
			
		}

		GameObjectContainer _container;
	};
}