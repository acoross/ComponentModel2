#pragma once

#include "scl/Types.h"
#include "scl/memory.h"
#include "scl/MathLib.h"
#include "scl/PhysicsLib.h"
#include "scl/Component.h"
#include "scl/EventDispatcher.h"
#include "GameEngine/LazyMotionObject.h"

namespace GameEngine
{
	using namespace scl;

	class IEventHandlerBinder
	{
	public:
		virtual void Bind() = 0;
	};

	class GameObject;
	
	class GameComponent : public Component<GameObject>
	{
	public:
		virtual ~GameComponent() {}

		Sp<GameObject> GetGameObject()
		{
			return GetOwner();
		}

		void OnBound()
		{
			for (auto& binder : _binders)
			{
				binder->Bind();
			}
		}

		void AddBinder(class IEventHandlerBinder* binder)
		{
			_binders.push_back(binder);
		}

	private:
		std::list<IEventHandlerBinder*> _binders;
	};

	class GameObject : public ComponentOwner<GameObject, GameComponent>
	{
	public:
		GameObject()
			: _id(scl::UniqueId <GameObject, scl::uint64>::Generate())
			, _rigidBody()
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
		void SendMsg(const Event<T>& message)
		{
			_eventDispatcher.InvokeEvent(message);
		}

		template <class T>
		void SendMsg(const T& msg)
		{
			_eventDispatcher.InvokeEvent(Event<T>(msg));
		}

		LazyRigidBody& RigidBody()
		{
			return _rigidBody;
		}

	private:
		const scl::uint64 _id;
		
		LazyRigidBody _rigidBody;

		Wp<class GameObjectContainer> _container;

		EventDispatcher _eventDispatcher;
	};
}