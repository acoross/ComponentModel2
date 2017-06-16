#include "GameComponent.h"

namespace GameEngine {

	void GameComponent::ForAllNeighbors(std::function<void(scl::Sp<GameObject>)> func)
	{
		if (auto gameObj = GetGameObject())
		{
			if (auto cont = gameObj->GetContainer())
			{
				for (auto pair : cont->GetAllObjects())
				{
					func(pair.second);
				}
			}
		}
	}
}