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
	장점: function 을 받으므로 유연하다. 가장 generic 한 형태이다.
	
	단점: event handler 일일히 등록해야 되서 귀찮음 (MessageDispatcher 와는 동일)
		Component 전용이 아니라서 사용시 헷갈릴 수도?

	의문: Component 를 receiver 로 고정해서 사용하는 것에 비해 casting 등이 부하가 될 수도?

	개발 상태: 일단 돌아가는 상태
	handler 를 list 로 들고있는데, MessageReceiverBox 같은 식으로 한번 감싸서 들고있게 변경
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