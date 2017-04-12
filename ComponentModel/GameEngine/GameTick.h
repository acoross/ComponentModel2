#pragma once

#include <cstdint>
#include <scl/IO/Pipeline.h>
#include <scl/System.h>

namespace GameEngine {
	class GameTick
	{
	public:
		static void Init()
		{
			_currentTick = 0;
			_deltaTime = 0;
		}

		static void UpdateTick(int64_t tick)
		{
			if (_currentTick != 0)
			{
				_deltaTime = tick - _currentTick;
			}
			_currentTick = tick;
		}

		static int64_t Tick()
		{
			return _currentTick;
		}

		static int64_t deltaTime()
		{
			return _deltaTime;
		}

		static int64_t _currentTick;
		static int64_t _deltaTime;
	};

	class GameTickUpdateTask : public scl::IThreadWorker
	{
	public:
		virtual void Run() final
		{
			int64_t tick = scl::System::GetTickMilliseconds();
			if (tick > GameTick::Tick() + 33)
			{
				GameTick::UpdateTick(tick);
			}
		}
	};
}