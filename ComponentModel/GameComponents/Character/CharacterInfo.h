#pragma once

#include "GameEngine\GameComponent.h"
#include "GameEngine\DataBinder.h"

namespace detail
{
	using namespace GameEngine;

	class CharacterInfo : public DataBinderComponent
	{
	public:
		DataMember<int64_t, 0, 10000> _hp{ this, 100 };
	};
}

using CharacterInfo = detail::CharacterInfo;