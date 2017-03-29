#pragma once

#include "GameEngine/GameComponent.h"
#include "GameEngine/Zone.h"

namespace GameEngine
{
	// collider 가 있는 GameObject 가 Zone 에 들어오면
	// collision check 대상으로 등록한다.

	// circle collider
	class Collider : public GameComponent
	{
	public:
		int Radius() const
		{
			return _radius;
		}

	private:
		float _radius;
		Wp<Zone> _zone;
	};
}