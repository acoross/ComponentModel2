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
		typedef std::unordered_map<int, Sp<GameObject>> MapType;

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

		template <class TMessage, 
			class = Require<IComponentMessage<TMessage>, TMessage>>
		void BroadcastMessage(Sp<TMessage> message)
		{
			for (auto& pair : _gameObjectMap)
			{
				pair.second->SendMessage_(message);
			}
		}

	private:
		std::unordered_map<int, Sp<GameObject>> _gameObjectMap;
	};
}