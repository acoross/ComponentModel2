#pragma once

#include "scl/memory.h"
#include "GameEngine/GameComponent.h"

namespace GameEngine
{
	template <class TComp>//, class = Require<GameComponent, T>>
	class GameComponentBinder
	{
	public:
		//static_assert(std::is_base_of<GameComponent, T>::value, "GameComponentBinder<T>: T should inherit GameCopmponent");

		GameComponentBinder(GameComponent* owner)
			: _owner(owner)
		{}

		TComp* operator->()
		{
			if (_comp.expired())
			{
				if (auto comp = _owner->GetComponent<TComp>())
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

	private:
		GameComponent* const _owner;
		scl::Wp<TComp> _comp;
	};
}