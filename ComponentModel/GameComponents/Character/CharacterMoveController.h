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
				// YawUpdater �� ȣ��Ǿ��ٴ� ���� �ڱ� �ڽ��� Transform �� ������ �ٴ� ��
				// �׷��Ƿ� �Ʒ����� &rbody �� �׻� ��ȿ�ϴ�.
				// ���� owner �� �����ϹǷ� Component �� �� ��ü (MoveController) �� ��ȿ�ϴ�.
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