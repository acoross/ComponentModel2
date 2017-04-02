#pragma once

#include "scl/EventDispatcher.h"

namespace GameEngine
{
	template <class T>
	class EventHandlerBinder : public IEventHandlerBinder
	{
	public:
		EventHandlerBinder(GameComponent& component, scl::EventDispatcher::TEventHandler<T> handler)
			: _component(component), _handler(handler)
		{
			_component.AddBinder(this);
		}

		void Bind() override sealed
		{
			_component.GetGameObject()->RegisterMsgHandler<T>(_handler);
		}

	private:
		GameComponent& _component;
		scl::EventDispatcher::TEventHandler<T> _handler;
	};
}