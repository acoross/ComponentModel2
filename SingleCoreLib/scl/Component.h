#pragma once

#include <map>
#include <typeinfo>
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
		
		Wp<TOwner> _owner;
	};

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
			auto id = typeid(TCompU).hash_code();
			auto ret = _components.emplace(id, comp);
			if (!ret.second)
			{
				throw std::exception();
			}

			comp->SetOwner(shared_from_this());
		}

		template <class TCompU, class X = Require<TComp, TCompU>>
		void SetComponent(Sp<TCompU> comp)
		{
			auto id = typeid(TCompU).hash_code();
			auto ret = _components.emplace(id, comp);
			if (!ret.second)
			{
				throw std::exception();
			}

			comp->SetOwner(shared_from_this());
		}

		template <class TCompU, class = Require<TComp, TCompU>>
		Sp<TCompU> GetComponent()
		{
			auto id = typeid(TCompU).hash_code();
			auto it = _components.find(id);
			if (it != _components.end())
			{
				return std::static_pointer_cast<TCompU>(it->second);
			}

			return nullptr;
		}

	private:
		std::map<size_t, Sp<TComp>> _components;
	};
}