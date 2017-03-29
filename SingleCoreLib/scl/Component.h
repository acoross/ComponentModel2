#pragma once

#include <map>
#include "scl/Types.h"
#include "scl/UniqueId.h"
#include "scl/memory.h"

namespace scl
{
	class IComponentMessageBase : public std::enable_shared_from_this<IComponentMessageBase>
	{
	public:
		template <class TMessage>
		struct MessageTypeIdHelper
		{
			static_assert(std::is_base_of<IComponentMessageBase, TMessage>::value,
				"ComponentTypeIdHelper<> should have template arg TComp derived from Component");

			static int typeId;
		};

		template <class TMessage, class = std::enable_if_t<std::is_base_of<IComponentMessageBase, TMessage>::value>>
		static int TypeId()
		{
			return MessageTypeIdHelper<std::remove_reference<TMessage>::type>::typeId;
		}

		template <class U, class = std::enable_if_t<std::is_base_of<IComponentMessageBase, U>::value>>
		bool Is()
		{
			auto myId = GetTypeId();
			auto uId = TypeId<U>();

			return myId == uId;
		}

		template <class U, class = std::enable_if_t<std::is_base_of<IComponentMessageBase, U>::value>>
		Sp<U> As()
		{
			if (Is<U>())
			{
				auto self = shared_from_this();
				return std::static_pointer_cast<U>();
			}
			return nullptr;
		}

	protected:
		virtual int GetTypeId() = 0;
	};

	template <class TMessage>
	int IComponentMessageBase::MessageTypeIdHelper<TMessage>::typeId = scl::UniqueId<IComponentMessageBase, int>::Generate();


	template <class T>
	class IComponentMessage : public IComponentMessageBase
	{
	public:
		int GetTypeId() override sealed
		{
			return TypeId<T>();
		}
	};

	// Component
	template <class TOwner>
	class Component
	{
	public:
		typedef Component<TOwner> TMy;

		/*static_assert(std::is_base_of<ComponentOwnerBase, TOwner>::value,
			"Component<TOwner> should have template arg TOwner based of ComponentOwnerBase");*/
	
		void SetOwner(Sp<TOwner> owner)
		{
			_owner = owner;
		}

		Sp<TOwner> GetOwner() const
		{
			return _owner.lock();
		}

		template <class TComp, class = std::enable_if_t<std::is_base_of<TMy, TComp>::value>>
		Sp<TComp> GetComponent()
		{
			if (auto owner = GetOwner())
			{
				return owner->GetComponent<TComp>();
			}

			return nullptr;
		}

		template <class TMessage, class = std::enable_if_t<std::is_base_of<IComponentMessage<TMessage>, TMessage>::value>>
		void SendMessage_(Sp<TMessage> message)
		{
			if (auto owner = GetOwner())
			{
				owner->SendMessage_(message);
			}
		}

		template <class TComp>
		struct ComponentTypeIdHelper
		{
			static_assert(std::is_base_of<TMy, TComp>::value,
				"ComponentTypeIdHelper<> should have template arg TComp derived from Component");

			static int typeId;
		};

		template <class TComp, class = std::enable_if_t<std::is_base_of<TMy, TComp>::value>>
		static int TypeId()
		{
			return ComponentTypeIdHelper<std::remove_reference<TComp>::type>::typeId;
		}
		
		Wp<TOwner> _owner;
	};

	template <class TOwner> template <class U>
	int Component<TOwner>::ComponentTypeIdHelper<U>::typeId = scl::UniqueId<Component<TOwner>, int>::Generate();


	// ComponentOwner
	template <class TOwner, class TComp>
	class ComponentOwner : public std::enable_shared_from_this<TOwner>
	{
	public:
		static_assert(std::is_base_of<Component<TOwner>, TComp>::value, "ComponentOwner<> template arg TComp should be derived from Component<TOwner>");

		typedef Component<TOwner> TMyComp;

		template <class TCompU, class X = std::enable_if_t<std::is_base_of<TComp, TCompU>::value>>
		void SetComponent()
		{
			auto comp = New<TCompU>();
			auto id = TMyComp::TypeId<TCompU>();
			_components.emplace(id, comp);
			comp->SetOwner(shared_from_this());
		}

		template <class TCompU, class... Args, class X = std::enable_if_t<std::is_base_of<TComp, TCompU>::value>>
		void SetComponent(Args&&... args)
		{
			auto comp = New<TCompU>(std::forward(args)...);
			auto id = TMyComp::TypeId<TCompU>();
			_components.emplace(id, comp);
			comp->SetOwner(shared_from_this());
		}

		template <class TCompU, class = std::enable_if_t<std::is_base_of<TComp, TCompU>::value>>
		Sp<TCompU> GetComponent()
		{
			auto id = TMyComp::TypeId<TCompU>();
			auto it = _components.find(id);
			if (it != _components.end())
			{
				return std::static_pointer_cast<TCompU>(it->second);
			}

			return nullptr;
		}

		// send message to components
		template <class TMessage, class = std::enable_if_t<std::is_base_of<IComponentMessage<TMessage>, TMessage>::value>>
		void SendMessage_(Sp<TMessage> message)
		{
		}

	private:
		std::map<int, Sp<TComp>> _components;
	};
}