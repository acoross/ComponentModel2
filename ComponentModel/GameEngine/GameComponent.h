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
	class IEventHandlerBinder
	{
	public:
		virtual ~IEventHandlerBinder() {}
		virtual void Bind() = 0;
	};

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
			for (auto& binder : _binders)
			{
				binder->Bind();
			}
		}

		void AddBinder(IEventHandlerBinder* binder)
		{
			_binders.push_back(binder);
		}

	private:
		std::list<IEventHandlerBinder*> _binders;
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

		template <class T>
		void RegisterMsgHandler(scl::EventDispatcher::TEventHandler<T> func)
		{
			_eventDispatcher.RegisterHandler(func);
		}

		template <class T>
		void SendMsg(const scl::Event<T>& message)
		{
			_eventDispatcher.InvokeEvent(message);
		}

		template <class T>
		void SendMsg(const T& msg)
		{
			_eventDispatcher.InvokeEvent(scl::Event<T>(msg));
		}

		LazyRigidBody& RigidBody()
		{
			return _rigidBody;
		}

	private:
		const scl::uint64 _id;
		
		LazyRigidBody _rigidBody;

		scl::Wp<class GameObjectContainer> _container;

		scl::EventDispatcher _eventDispatcher;
	};

	template <class T>//, class = Require<GameComponent, T>>
	class GameComponentBinder
	{
	public:
		//static_assert(std::is_base_of<GameComponent, T>::value, "GameComponentBinder<T>: T should inherit GameCopmponent");

		GameComponentBinder(GameComponent* owner)
			: _owner(owner)
		{}

		T* operator->()
		{
			if (_comp.expired())
			{
				if (auto comp = _owner->GetComponent<T>())
				{
					_comp = comp;
					return comp.get();
				}
			}
			else
			{
				if (auto comp = _comp.lock())
				{
					return comp.get();
				}
			}

			return nullptr;
		}

		GameComponent* const _owner;
		scl::Wp<T> _comp;
	};
}