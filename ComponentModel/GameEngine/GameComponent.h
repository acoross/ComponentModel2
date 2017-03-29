#pragma once

#include "Types.h"
#include "memory.h"
#include "MathLib.h"
#include "PhysicsLib.h"
#include "Component.h"

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

		const scl::uint64 _id;
		scl::Transform _transform;
	};
}