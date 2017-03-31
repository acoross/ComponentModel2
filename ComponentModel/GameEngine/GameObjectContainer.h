#pragma once

#include <map>

#include "memory.h"
#include <unordered_map>
#include "./GameComponent.h"
#include "scl/TypeTraits.h"

namespace GameEngine
{
	using namespace scl;

	class GameObjectContainer
	{
	public:
		typedef std::unordered_map<uint64, Sp<GameObject>> MapType;

		const MapType& GetAllObjects() const
		{
			return _gameObjectMap;
		}
		
		void Add(Sp<GameObject> gameObject)
		{
			if (!gameObject) return;
			_gameObjectMap[gameObject->Id()] = gameObject;
		}

		void Remove(Sp<GameObject> gameObject)
		{
			if (!gameObject) return;
			_gameObjectMap.erase(gameObject->Id());
		}

		template <class TMessage>
		void BroadcastMsg(const TMessage& message)
		{
			for (auto& pair : _gameObjectMap)
			{
				pair.second->SendMsg(message);
			}
		}

		template <class TMessage>
		void BroadcastMsg(const Event<TMessage>& message)
		{
			for (auto& pair : _gameObjectMap)
			{
				pair.second->SendMsg(message);
			}
		}

	private:
		std::unordered_map<uint64, Sp<GameObject>> _gameObjectMap;
	};
}