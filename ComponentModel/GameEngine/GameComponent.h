#pragma once

#include <typeinfo>
#include <numeric>
#include <unordered_map>

#include "scl/Types.h"
#include "scl/TypeInfo.h"
#include "scl/memory.h"
#include "scl/MathLib.h"
#include "scl/PhysicsLib.h"
#include "scl/Component.h"
#include "scl/exception.h"
#include "GameEngine/LazyTransform.h"

namespace GameEngine
{
	class GameObject;
	
	class GameComponent : public scl::Component<GameObject>
	{
	public:
		virtual ~GameComponent() {}

		scl::Sp<GameObject> GetGameObject()
		{
			return GetOwner();
		}

		template <class TComp, class = Require<GameComponent, TComp>>
		static size_t GetTypeId()
		{
			return scl::TypeId<TComp>();
		}

		static size_t GetTypeId(GameComponent& obj)
		{
			return scl::TypeId(obj);
		}

		void ForAllNeighbors(std::function<void(scl::Sp<GameObject>)> func);
	};

	class GameObject : public scl::ComponentOwner<GameObject, GameComponent>
	{
	public:
		GameObject()
			: _id(scl::UniqueId<GameObject, scl::uint64>::Generate())
			, _transform()
		{}

		scl::uint64 Id() const
		{
			return _id;
		}

		scl::Sp<class GameObjectContainer> GetContainer()
		{
			return _container.lock();
		}

		LazyTransform& Transform()
		{
			return _transform;
		}

	private:
		const scl::uint64 _id;
		
		LazyTransform _transform;

		scl::Wp<class GameObjectContainer> _container;
	};

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