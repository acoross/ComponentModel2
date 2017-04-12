#pragma once

#include "GameEngine\GameComponent.h"
#include "GameEngine\DataObserver.h"

namespace GameEngine
{
	class DataBinderComponent : public GameComponent
	{
	public:
		virtual void OnBound() final
		{
			for (auto pair : _dataBinderOnBind)
			{
				pair.second();
			}

			OnInit();
		}

		virtual void OnInit() {}

	private:
		std::unordered_map<int, std::function<void()>> _dataBinderOnBind;
	};

	template <class T, T TMin, T TMax, class = std::enable_if_t<std::is_arithmetic<T>::value>>
	class DataMember
	{
	public:
		DataMember(DataBinderComponent* owner, int code, T init = TMin)
			: _owner(owner)
			, _code(code)
			, _val(init)
		{
			static_assert(TMin <= TMax);

			_owner->_dataBinderOnBind[code] = [this]() {
				Inform();
			};
		}

		T Get() const
		{
			return _val;
		}

		T Set(T val)
		{
			_val = clamp(val);
			Inform();
			return _val;
		}

		T TrySet(T val)
		{
			if (isInRange(val))
			{
				_val = val;
				Inform();
			}
			return _val;
		}

		T Add(T diff)
		{
			_val = clamp(_val + diff);
			InformDiff(diff);
			return _val;
		}

		T TryAdd(T diff)
		{
			if (isInRange(_val + diff))
			{
				_val += diff;
				InformDiff(diff);
			}
			return _val;
		}

		void Inform()
		{
			auto objId = _owner->GetGameObject()->Id();
			_owner->ForAllNeighbors([this, &](scl::Sp<GameObject> obj)
			{
				if (auto observer = obj->GetComponent<IDataObserver>())
				{
					observer->OnChanged(objId, _code, false, _val);
				}
			});
		}

	private:
		void InformDiff(T diff)
		{
			auto objId = _owner->GetGameObject()->Id();
			_owner->ForAllNeighbors([this, &](scl::Sp<GameObject> obj)
			{
				if (auto observer = obj->GetComponent<IDataObserver>())
				{
					observer->OnChanged(objId, _code, true, diff);
				}
			});
		}

		static bool isInRange(T val)
		{
			if (val > TMax)
				return false;
			if (val < TMin)
				return false;

			return true;
		}

		static int clamp(T val)
		{
			if (val > TMax)
				return TMax;
			if (val < TMin)
				return TMin;
			return val;
		}

	private:
		DataBinderComponent* const _owner;
		const int _code;
		T _val{ TMin };
	};

}