#pragma once

#include "GameEngine/GameComponent.h"
#include "GameEngine/EventHandlerBinder.h"

namespace UnitTest
{
	class Component2 : public GameEngine::GameComponent
	{
	public:
		float val{ 0.f };
		GameEngine::GameComponentBinder<class Component1> _comp1{ this };

		int getComp1Val();
	};
}