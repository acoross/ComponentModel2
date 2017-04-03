#pragma once

#include <map>
#include <unordered_map>

#include "scl/memory.h"
#include "scl/TypeTraits.h"
#include "GameEngine/GameComponent.h"

namespace GameEngine
{
	class GameObjectContainer
	{
	public:
		typedef std::unordered_map<scl::uint64, scl::Sp<GameObject>> MapType;

		const MapType& GetAllObjects() const
		{
			return _gameObjectMap;
		}
		
		void Add(scl::Sp<GameObject> gameObject)
		{
			if (!gameObject) return;
			_gameObjectMap[gameObject->Id()] = gameObject;
		}

		void Remove(scl::Sp<GameObject> gameObject)
		{
			if (!gameObject) return;
			_gameObjectMap.erase(gameObject->Id());
		}

	private:
		std::unordered_map<scl::uint64, scl::Sp<GameObject>> _gameObjectMap;
	};
}