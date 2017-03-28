#pragma once

#include <map>
#include "Types.h"
#include "UniqueId.h"
#include "memory.h"

namespace GameEngine
{
	using namespace scl;
	
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

	template <class TOwner, class TComp>
		//, class X = std::enable_if_t<std::is_base_of<Component<TOwner>, TComp>::value>>
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

		std::map<int, Sp<TComp>> _components;
	};
}