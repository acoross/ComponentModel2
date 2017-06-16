#pragma once

#include <functional>
#include "scl/MathLib.h"
#include "scl/Types.h"
#include "scl/TemplateHelper.h"
#include "GameEngine/GameTick.h"

namespace GameEngine
{
	class LazyTransform : public scl::NoCopy<LazyTransform>
	{
		using Vector3f = scl::Vector3f;

	public:
		using PositionUpdaterFunc = 
			std::function<Vector3f(
				const Vector3f& /*pos*/, const float& /*yaw*/, const Vector3f& /*vel*/, const float /*ang vel*/, scl::int64 /*last updated*/)>;
		using YawUpdaterFunc =
			std::function<float(
				const Vector3f& /*pos*/, const float& /*yaw*/, const Vector3f& /*vel*/, const float /*ang vel*/, scl::int64 /*last updated*/)>;
		
		LazyTransform()
			: _lastUpdated(GameTick::Tick())
		{}

		LazyTransform(Vector3f pos, float yaw, Vector3f vel, float angVel)
			: _position(pos), _yaw(yaw), _velocity(vel), _angVelocity(angVel), _lastUpdated(GameTick::Tick())
		{}

		void SetPositionUpdater(PositionUpdaterFunc updater)
		{
			_positionUpdater = updater;
		}

		void SetYawUpdater(YawUpdaterFunc updater)
		{
			_yawUpdater = updater;
		}

		void SetPosition(Vector3f pos)
		{
			_position = pos;
			_lastUpdated = GameTick::Tick();
		}

		void SetYaw(float yaw)
		{
			_yaw = yaw;
			_lastUpdated = GameTick::Tick();
		}

		void SetVelocity(Vector3f vel)
		{
			_velocity = vel;
			_lastUpdated = GameTick::Tick();
		}

		void SetAngVelocity(float angVel)
		{
			_angVelocity = angVel;
			_lastUpdated = GameTick::Tick();
		}

		Vector3f Position()
		{
			update();
			return _position;
		}

		float Yaw()
		{
			update();
			return scl::Math::AdjustAngle(_yaw);
		}

	private:
		void update()
		{
			auto gametick = GameTick::Tick();
			if (gametick == 0)
			{
				return;
			}

			if (_lastUpdated == 0)
			{
				_lastUpdated = gametick;
				return;
			}

			if (_lastUpdated == gametick)
				return;

			auto diffms = gametick - _lastUpdated;
			if (diffms < 0)
				throw std::exception("invalid time value!");

			_position = _position + _velocity * diffms;
			_yaw = _yaw + _angVelocity * diffms;

			auto lastUpdatedBackup = _lastUpdated;
			_lastUpdated = gametick;

			if (_positionUpdater) 
			{ 
				_position = _positionUpdater(_position, _yaw, _velocity, _angVelocity, lastUpdatedBackup);
			}
			if (_yawUpdater) 
			{ 
				_yaw = _yawUpdater(_position, _yaw, _velocity, _angVelocity, lastUpdatedBackup);
			}
		}

		Vector3f _position{ 0, 0, 0 };	// cm
		float _yaw{ 0 };				// degree

		Vector3f _velocity{ 0, 0, 0 };	// cm / ms
		float _angVelocity{ 0 };	// degree / ms

		scl::int64 _lastUpdated;

		PositionUpdaterFunc _positionUpdater{ nullptr };
		YawUpdaterFunc _yawUpdater{ nullptr };
	};
}