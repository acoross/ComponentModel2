#pragma once

#include <map>

#include "memory.h"
#include <unordered_map>
#include "./GameComponent.h"

namespace GameEngine
{
	class GameObjectContainer
	{
	public:
		typedef std::unordered_map<int, Sp<GameObject>> MapType;

		const MapType& GetAllObjects() const
		{
			return _gameObjectMap;
		}

		void ForAll()
		{

		}

	private:
		std::unordered_map<int, Sp<GameObject>> _gameObjectMap;
	};
}