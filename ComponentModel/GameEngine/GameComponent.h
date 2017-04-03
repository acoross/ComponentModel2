#pragma once

#include <typeinfo>

#include "scl/Types.h"
#include "scl/TypeInfo.h"
#include "scl/memory.h"
#include "scl/MathLib.h"
#include "scl/PhysicsLib.h"
#include "scl/Component.h"
#include "scl/exception.h"
#include "GameEngine/LazyMotionObject.h"

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

		virtual void OnBound() override
		{
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
	};

	class GameObject : public scl::ComponentOwner<GameObject, GameComponent>
	{
	public:
		GameObject()
			: _id(scl::UniqueId<GameObject, scl::uint64>::Generate())
			, _rigidBody()
		{}

		scl::uint64 Id() const
		{
			return _id;
		}

		scl::Sp<class GameObjectContainer> GetContainer()
		{
			return _container.lock();
		}

		LazyRigidBody& RigidBody()
		{
			return _rigidBody;
		}

	private:
		const scl::uint64 _id;
		
		LazyRigidBody _rigidBody;

		scl::Wp<class GameObjectContainer> _container;
	};
}