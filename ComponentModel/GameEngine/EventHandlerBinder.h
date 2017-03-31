#pragma once

#include "scl/EventDispatcher.h"
using namespace scl;

#include "scl/Component.h"

namespace GameEngine
{
	template <class T>
	class EventHandlerBinder : public IEventHandlerBinder
	{
	public:
		EventHandlerBinder(GameComponent& component, EventDispatcher::TEventHandler<T> handler)
			: _component(component), _handler(handler)
		{
			_component.AddBinder(this);
		}

		void Bind() override sealed
		{
			_component.GetGameObject()->RegisterMsgHandler<T>(_handler);
		}

		GameComponent& _component;
		EventDispatcher::TEventHandler<T> _handler;
	};
}