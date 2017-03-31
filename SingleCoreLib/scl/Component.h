#pragma once

#include <map>
#include "scl/Types.h"
#include "scl/UniqueId.h"
#include "scl/memory.h"
#include "scl/TypeTraits.h"

namespace scl
{
	// Component
	template <class TOwner>
	class Component
	{
	public:
		typedef Component<TOwner> TMy;

		/*static_assert(std::is_base_of<ComponentOwnerBase, TOwner>::value,
			"Component<TOwner> should have template arg TOwner based of ComponentOwnerBase");*/
		
		virtual ~Component(){}

		virtual void OnBound() {};

		void SetOwner(Sp<TOwner> owner)
		{
			_owner = owner;
			OnBound();
		}

		Sp<TOwner> GetOwner() const
		{
			return _owner.lock();
		}

		template <class TComp, class = Require<TMy, TComp>>
		Sp<TComp> GetComponent()
		{
			if (auto owner = GetOwner())
			{
				return owner->GetComponent<TComp>();
			}

			return nullptr;
		}

		template <class TComp>
		struct ComponentTypeIdHelper
		{
			static_assert(std::is_base_of<TMy, TComp>::value,
				"ComponentTypeIdHelper<> should have template arg TComp derived from Component");

			static int typeId;
		};

		template <class TComp, class = Require<TMy, TComp>>
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

		template <class TCompU, class X = Require<TComp, TCompU>>
		void SetComponent()
		{
			auto comp = New<TCompU>();
			auto id = TMyComp::TypeId<TCompU>();
			_components.emplace(id, comp);
			comp->SetOwner(shared_from_this());
		}

		template <class TCompU, class X = Require<TComp, TCompU>>
		void SetComponent(Sp<TCompU> comp)
		{
			auto id = TMyComp::TypeId<TCompU>();
			_components.emplace(id, comp);
			comp->SetOwner(shared_from_this());
		}

		template <class TCompU, class = Require<TComp, TCompU>>
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

	private:
		std::map<int, Sp<TComp>> _components;
	};
}