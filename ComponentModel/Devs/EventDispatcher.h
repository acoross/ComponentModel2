#pragma once

#include <type_traits>
#include <unordered_map>
#include <vector>
#include <functional>
#include "scl/UniqueId.h"
#include "scl/TypeTraits.h"
#include "TypeInfo.h"

namespace devs
{
	/*
	����: function �� �����Ƿ� �����ϴ�. ���� generic �� �����̴�.
	
	����: event handler ������ ����ؾ� �Ǽ� ������ (MessageDispatcher �ʹ� ����)
		Component ������ �ƴ϶� ���� �򰥸� ����?

	�ǹ�: Component �� receiver �� �����ؼ� ����ϴ� �Ϳ� ���� casting ���� ���ϰ� �� ����?

	���� ����: �ϴ� ���ư��� ����
	handler �� list �� ����ִµ�, MessageReceiverBox ���� ������ �ѹ� ���μ� ����ְ� ����
	*/


	using namespace scl;

	class IEvent
	{
	public:
		virtual ~IEvent() {}
	};

	template <class T>
	class Event : public IEvent
	{
	public:
		Event() {}
		Event(T v) : value(v) {}

		T value;
	};

	class EventDispatcher
	{
	public:
		template <class T>
		using TEventHandler = std::function<void(const T&)>;

		using EventHandlerFunc = TEventHandler<IEvent>;

		template <class T, class = Require<IEvent, T>>
		static size_t GetEventType()
		{
			return scl::TypeId<T>();
		}

		template <class T>
		void RegisterHandler(TEventHandler<T> func)
		{
			auto handler = [func](const IEvent& e)
			{
				auto& evt = dynamic_cast<const Event<T>&>(e);
				func(evt.value);
			};

			auto typeId = GetEventType<Event<T>>();
			auto it = _handlerListMap.find(typeId);
			if (it != _handlerListMap.end())
			{
				it->second.emplace_back(std::move(handler));
			}
			else
			{
				std::vector<EventHandlerFunc> list;
				list.emplace_back(std::move(handler));
				_handlerListMap.emplace(typeId, std::move(list));
			}
		}

		template <class T>
		void InvokeEvent(const Event<T>& evt)
		{
			auto typeId = GetEventType<Event<T>>();
			auto it = _handlerListMap.find(typeId);
			if (it != _handlerListMap.end())
			{
				for (auto handler : it->second)
				{
					handler(evt);
				}
			}
		}

		template <class T>
		void operator() (const T& evt)
		{
			InvokeEvent(evt);
		}

		std::unordered_map<size_t, std::vector<EventHandlerFunc>> _handlerListMap;
	};
}