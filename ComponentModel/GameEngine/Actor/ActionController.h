#pragma once

#include "scl/memory.h"
#include "GameEngine/GameComponent.h"

namespace GameEngine
{
	/*
	  Action
		* 한 번에 하나만 처리할 수 있다.
		* 여러 프레임에 걸쳐서 처리된다.
		* 주위에 Action 이 처리되는 상황을 알린다.
		* 중간에 취소할 수 있다.
	*/

	class IAction
	{
	public:
		virtual ~IAction() {}

		virtual bool CheckTransition(const IAction& other) const = 0;
		virtual void Cancel() = 0;

		virtual bool CheckStartCondition() const = 0;
		virtual bool Start() = 0;
	};

	class IActionObserver : public GameComponent
	{
	public:
		virtual ~IActionObserver() {}
		virtual void OnObserveAction(int64_t objId, const IAction& action) = 0;
	};

	class ActionController : public GameComponent
	{
	public:
		bool RequestAction(scl::Sp<IAction> action)
		{
			if (_currentAction)
			{
				if (!_currentAction->CheckTransition(*action))
				{
					return false;
				}

				_currentAction->Cancel();
			}

			if (!action->CheckStartCondition())
			{
				return false;
			}

			action->Start();

			auto objId = GetGameObject()->Id();
			ForAllNeighbors([&action, objId](scl::Sp<GameObject> obj)
			{
				if (auto obs = obj->GetComponent<IActionObserver>())
				{
					obs->OnObserveAction(objId, *action);
				}
			});

			return true;
		}
		
	private:
		scl::Sp<IAction> _currentAction;
	};
}