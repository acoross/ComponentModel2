#pragma once

#include "GameEngine/GameComponent.h"
#include "GameEngine/Zone.h"

namespace GameEngine
{
	// collider �� �ִ� GameObject �� Zone �� ������
	// collision check ������� ����Ѵ�.

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