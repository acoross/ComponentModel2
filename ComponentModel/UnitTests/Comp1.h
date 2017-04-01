#pragma once

#include "GameEngine/GameComponent.h"

namespace UnitTest
{
	class Component1 : public GameEngine::GameComponent
	{
	public:
		Component1();

		int val{ 0 };

		GameEngine::ComponentBinder<class Component2> _comp2{ this };

		float GetComp2Val();
	};
}