#pragma once

#include <functional>
#include "scl/MathLib.h"
#include "scl/Types.h"
#include "scl/TemplateHelper.h"

namespace GameEngine
{
	class LazyRigidBody : public scl::NoCopy<LazyRigidBody>
	{
		using Vector3f = scl::Vector3f;

	public:
		using PositionUpdaterFunc = 
			std::function<Vector3f(
				const Vector3f& /*pos*/, const float& /*yaw*/, const Vector3f& /*vel*/, const float /*ang vel*/, scl::int64 /*last updated*/, scl::int64 /*currentTick*/)>;
		using YawUpdaterFunc =
			std::function<float(
				const Vector3f& /*pos*/, const float& /*yaw*/, const Vector3f& /*vel*/, const float /*ang vel*/, scl::int64 /*last updated*/, scl::int64 /*currentTick*/)>;
		
		LazyRigidBody(scl::int64 gametick = 0)
			: _lastUpdated(gametick)
		{}

		LazyRigidBody(Vector3f pos, float yaw, Vector3f vel, float angVel, scl::int64 gametick = 0)
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

		void SetPosition(Vector3f pos, scl::int64 gametick)
		{
			_position = pos;
			_lastUpdated = gametick;
		}

		void SetYaw(float yaw, scl::int64 gametick)
		{
			_yaw = yaw;
			_lastUpdated = gametick;
		}

		void SetVelocity(Vector3f vel, scl::int64 gametick)
		{
			_velocity = vel;
			_lastUpdated = gametick;
		}

		void SetAngVelocity(float angVel, scl::int64 gametick)
		{
			_angVelocity = angVel;
			_lastUpdated = gametick;
		}

		Vector3f Position(scl::int64 gametick)
		{
			update(gametick);
			return _position;
		}

		float Yaw(scl::int64 gametick)
		{
			update(gametick);
			return scl::Math::AdjustAngle(_yaw);
		}

	private:
		void update(scl::int64 gametick)
		{
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

			if (_positionUpdater) 
			{ 
				_position = _positionUpdater(_position, _yaw, _velocity, _angVelocity, _lastUpdated, gametick);
			}
			if (_yawUpdater) 
			{ 
				_yaw = _yawUpdater(_position, _yaw, _velocity, _angVelocity, _lastUpdated, gametick);
			}

			_lastUpdated = gametick;
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