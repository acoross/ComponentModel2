#pragma once

#include <functional>
#include <unordered_map>

#include "GameEngine\GameComponent.h"

namespace GameEngine
{
	class IDataObserver : public GameComponent
	{
	public:
		virtual ~IDataObserver() {}

		virtual void OnChanged(int64_t objId, int code, bool isDiff, int64_t val) = 0;
		virtual void OnChanged(int64_t objId, int code, bool isDiff, float val) = 0;
	};
}