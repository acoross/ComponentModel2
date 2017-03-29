#pragma once

#include "scl/Types.h"
#include "scl/memory.h"
#include "scl/MathLib.h"
#include "scl/PhysicsLib.h"
#include "scl/Component.h"

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