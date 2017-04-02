#pragma once

#include "GameEngine/GameComponent.h"

namespace GameEngine
{
	class CharacterMoveController : public GameComponent
	{
		using Vector3f = scl::Vector3f;

	public:
		virtual ~CharacterMoveController() {}

		virtual void OnBound() override
		{
			GameComponent::OnBound();

			if (auto gameObject = GetGameObject())
			{
				auto& rbody = gameObject->RigidBody();
				// YawUpdater 가 호출되었다는 것은 자기 자신인 RigidBody 도 존재한 다는 뜻
				// 그러므로 아래에서 &rbody 는 항상 유효하다.
				// 또한 owner 가 존재하므로 Component 인 이 객체 (MoveController) 도 유효하다.
				rbody.SetYawUpdater(
					[&rbody, this](
						const Vector3f& position, const float& yaw, const Vector3f& vel, float angVel, scl::int64 lastTick, scl::int64 currentTick)
					->float
				{
					if (auto target = _target.lock())
					{
						auto myPos = rbody.Position(0);
						auto tPos = target->RigidBody().Position(currentTick);
						auto direction = tPos - myPos;

						return direction.Yaw();
					}

					return yaw;
				});
			}
		}

		void SetTarget(scl::Sp<GameObject> target)
		{
			if (target)
			{
				_targetObjId = target->Id();
				_target = target;
			}
			else
			{
				RemoveTarget();
			}
		}

		void RemoveTarget()
		{
			_targetObjId = 0;
			_target.reset();
		}

		scl::uint64 _targetObjId;
		scl::Wp<GameObject> _target;
	};
}