#pragma once

#include <functional>
#include "scl/MathLib.h"
#include "scl/Types.h"

using namespace scl;

namespace GameEngine
{
	class LazyRigidBody
	{
	public:
		using PositionUpdaterFunc = std::function<void(Vector3f& /*pos*/, const Vector3f& /*vel*/, int64 /*diff*/)>;
		using YawUpdaterFunc = std::function<void(float& /*yaw*/, const float /*ang vel*/, int64 /*diff*/)>;

		LazyRigidBody()
		{}

		LazyRigidBody(Vector3f pos, float yaw, Vector3f vel, float angVel, int64 gametick = 0)
			: _position(pos), _yaw(yaw), _velocity(vel), _angVelocity(angVel), _lastUpdated(gametick)
		{}

		void SetPositionUpdater(PositionUpdaterFunc updater)
		{
			_positionUpdater = updater;
		}

		void SetYawUpdater(YawUpdaterFunc updater)
		{
			_yawUpdater = updater;
		}

		void SetPosition(Vector3f pos, int64 gametick)
		{
			_position = pos;
			_lastUpdated = gametick;
		}

		void SetYaw(float yaw, int64 gametick)
		{
			_yaw = yaw;
			_lastUpdated = gametick;
		}

		void SetVelocity(Vector3f vel, int64 gametick)
		{
			_velocity = vel;
			_lastUpdated = gametick;
		}

		void SetAngVelocity(float angVel, int64 gametick)
		{
			_angVelocity = angVel;
			_lastUpdated = gametick;
		}

		Vector3f Position(int64 gametick)
		{
			update(gametick);
			return _position;
		}

		float Yaw(int64 gametick)
		{
			update(gametick);
			return Math::AdjustAngle(_yaw);
		}

	private:
		void update(int64 gametick)
		{
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

			if (_positionUpdater) { _positionUpdater(_position, _velocity, diffms); }
			else { _position = _position + _velocity * diffms; }
			
			if (_yawUpdater) { _yawUpdater(_yaw, _angVelocity, diffms); }
			else { _yaw = _yaw + _angVelocity * diffms; }

			_lastUpdated = gametick;
		}

		Vector3f _position{ 0, 0, 0 };	// cm
		float _yaw{ 0 };				// degree

		Vector3f _velocity{ 0, 0, 0 };	// cm / ms
		float _angVelocity{ 0 };	// degree / ms

		int64 _lastUpdated;

		PositionUpdaterFunc _positionUpdater{ nullptr };
		YawUpdaterFunc _yawUpdater{ nullptr };
	};
}