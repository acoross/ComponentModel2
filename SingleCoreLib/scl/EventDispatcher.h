#pragma once

#include <type_traits>
#include <unordered_map>
#include <vector>
#include <functional>
#include "scl/UniqueId.h"

namespace scl
{
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
		class EventTypeIdGenerator
		{
		public:
			static int typeId;
		};

		template <class T, class = std::enable_if_t<std::is_base_of<IEvent, T>::value>>
		static int GetEventType()
		{
			return EventTypeIdGenerator<T>::typeId;
		}

		template <class T, class = std::enable_if_t<std::is_base_of<IEvent, T>::value>>
		void RegisterHandler(std::function<void(const T&)> func)
		{
			auto handler = [func](const IEvent& e)
			{
				auto& evt = dynamic_cast<const T&>(e);
				func(evt);
			};

			auto typeId = GetEventType<T>();
			auto it = _handlerListMap.find(typeId);
			if (it != _handlerListMap.end())
			{
				it->second.emplace_back(std::move(handler));
			}
			else
			{
				std::vector<TEventHandler> list;
				list.emplace_back(std::move(handler));
				_handlerListMap.emplace(typeId, std::move(list));
			}
		}

		template <class T, class = std::enable_if_t<std::is_base_of<IEvent, T>::value>>
		void InvokeEvent(const T& evt)
		{
			auto typeId = GetEventType<T>();
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

		using TEventHandler = std::function<void(const IEvent&)>;
		std::unordered_map<int, std::vector<TEventHandler>> _handlerListMap;
	};

	template <class T>
	int EventDispatcher::EventTypeIdGenerator<T>::typeId = scl::UniqueId<EventDispatcher, int>::Generate();
}