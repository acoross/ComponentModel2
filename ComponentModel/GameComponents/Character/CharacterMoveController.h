#pragma once

#include "GameEngine/GameComponent.h"

namespace detail
{
	using namespace GameEngine;

	class CharacterMoveController : public GameComponent
	{
		using Vector3f = scl::Vector3f;

	public:
		virtual ~CharacterMoveController() {}

		virtual void OnBound() override
		{
			if (auto gameObject = GetGameObject())
			{
				auto& rbody = gameObject->Transform();
				// YawUpdater 가 호출되었다는 것은 자기 자신인 Transform 도 존재한 다는 뜻
				// 그러므로 아래에서 &rbody 는 항상 유효하다.
				// 또한 owner 가 존재하므로 Component 인 이 객체 (MoveController) 도 유효하다.
				rbody.SetYawUpdater(
					[&rbody, this](
						const Vector3f& position, const float& yaw, const Vector3f& vel, float angVel, scl::int64 lastTick)
					->float
				{
					if (auto target = _target.lock())
					{
						Vector3f myPos = rbody.Position();
						Vector3f tPos = target->Transform().Position();
						Vector3f direction = tPos - myPos;

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

		void Move(Vector3f pos, Vector3f velocity, float yaw)
		{
			
		}

		scl::uint64 _targetObjId;
		scl::Wp<GameObject> _target;
	};
}

using CharacterMoveController = detail::CharacterMoveController;