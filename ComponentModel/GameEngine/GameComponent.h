#pragma once

#include "scl/Types.h"
#include "scl/memory.h"
#include "scl/MathLib.h"
#include "scl/PhysicsLib.h"
#include "scl/Component.h"
#include "scl/EventDispatcher.h"

namespace GameEngine
{
	using namespace scl;

	class GameObject;
	
	class GameComponent : public Component<GameObject>
	{
	public:
		Sp<GameObject> GetGameObject()
		{
			return GetOwner();
		}
	};

	class GameObject : public ComponentOwner<GameObject, GameComponent>
	{
	public:
		GameObject()
			: _id(scl::UniqueId <GameObject, scl::uint64>::Generate())
		{}

		uint64 Id() const
		{
			return _id;
		}

		Sp<class GameObjectContainer> GetContainer()
		{
			return _container.lock();
		}

		template <class T>
		void RegisterMsgHandler(EventDispatcher::TEventHandler<T> func)
		{
			_eventDispatcher.RegisterHandler(func);
		}

		template <class T>
		void SendMsg(Event<T>&& message)
		{
			_eventDispatcher.InvokeEvent(std::forward<Event<T>>(message));
		}

		template <class T>
		void SendMsg(T&& msg)
		{
			_eventDispatcher.InvokeEvent(Event<T>(std::forward<T>(msg)));
		}

	private:
		const scl::uint64 _id;
		
		scl::Transform _transform;

		Wp<class GameObjectContainer> _container;

		EventDispatcher _eventDispatcher;
	};
}