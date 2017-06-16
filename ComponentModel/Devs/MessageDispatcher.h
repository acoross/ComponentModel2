#pragma once

#include <map>
#include "scl/memory.h"
#include "scl/TypeTraits.h"
#include "scl/TypeInfo.h"
#include "scl/exception.h"
#include "GameEngine/GameComponent.h"

namespace devs
{
	/*
	장점: Component 를 통째로 이용
			generic 과 specialized 사이에 있는 듯.
			성능과 편의성도 중간?
	
	단점: Handler 를 정의하고 event 등록을 안 할수 있다. 
		Handler 형식이 HandleMessage(TMsg& ) 로 고정되어서, Handler 정의와 Handler 등록이 분리된다.

	개발 상태: Box 에 Comp 붙일 때 원래 타입으로 넣어야 한다. (지금은 GameComponent)
		이 부분에 function 으로 wrapping 필요할 듯
	*/

	using namespace GameEngine;

	class IReceiverBox
	{
	public:
		virtual ~IReceiverBox() {}
	};

	template <class TMsg> //, class = scl::Require<IMsg, TMsg>>
	class MessageReceiverBox : public IReceiverBox
	{
	public:
		virtual ~MessageReceiverBox() {}

		void AddReceiver(scl::Sp<GameComponent> comp)
		{
			size_t typeId = GameComponent::GetTypeId(*comp);

			auto ret = _receivers.emplace(typeId, comp);
			if (!ret.second)
			{
				scl::Exception::RaiseException();
			}
		}

		void SendMsg(scl::Sp<GameComponent> sender, const TMsg& msg)
		{
			size_t senderType = GameComponent::GetTypeId(*sender);

			for (auto pair : _receivers)
			{
				if (pair.first != senderType)
				{
					if (auto comp = pair.second->lock())
					{
						comp->HandleMsg(msg);
					}
				}
			}
		}

	private:
		std::map<size_t, scl::Wp<GameComponent>> _receivers;
	};

	class MessageDispatcher
	{
	public:
		template <class TMsg>
		static size_t MsgType(const TMsg& msg)
		{
			return scl::TypeId(msg);
		}

		template <class TMsg>
		static size_t MsgType()
		{
			return scl::TypeId<TMsg>();
		}

		template <class TMsg, class TComp,
			//class = scl::Require<IMsg, TMsg>, 
			class = scl::Require<GameComponent, TComp>>
			void AddReceiver(scl::Sp<TComp> comp)
		{
			auto box = GetReceiverBox<TMsg>();
			if (!box)
			{
				box = CreateReceiverBox<TMsg>();
			}

			box->AddReceiver(comp);
		}

		template <class TMsg, class TComp,
			//class = scl::Require<IMsg, TMsg>, 
			class = scl::Require<GameComponent, TComp>>
			void SendMsg(scl::Sp<TComp> sender, const TMsg& msg)
		{
			if (auto box = GetReceiverBox<TMsg>())
			{
				box->SendMsg(sender, msg);
			}
		}

	private:
		template <class TMsg>
			//class = scl::Require<IMsg, TMsg>>
			scl::Sp<MessageReceiverBox<TMsg>> GetReceiverBox()
		{
			size_t msgType = MsgType<TMsg>();
			
			auto it = _msgTypeToBox.find(msgType);
			if (it != _msgTypeToBox.end())
			{
				return std::dynamic_pointer_cast<MessageReceiverBox<TMsg>>(it->second);
			}
			
			return nullptr;
		}

		template <class TMsg>
			//class = scl::Require<IMsg, TMsg>>
			scl::Sp<MessageReceiverBox<TMsg>> CreateReceiverBox()
		{
			size_t msgType = MsgType<TMsg>();

			auto newBox = New<MessageReceiverBox<TMsg>>();
			auto ret = _msgTypeToBox.emplace(msgType, newBox);
			if (!ret.second)
			{
				scl::Exception::RaiseException();
			}

			return newBox;
		}

		std::map<size_t, scl::Sp<IReceiverBox>> _msgTypeToBox;
	};

	//usage
	inline void msgdispatch_sample()
	{
		MessageReceiverBox<int> box;

		class Comp1 : public GameComponent, std::enable_shared_from_this<Comp1>
		{
		public:
			virtual void OnBound() final
			{
				
			}

			void OnInit(MessageReceiverBox<int>& box)
			{
				box.AddReceiver(shared_from_this());
			}

			void HandleMsg(int msg)
			{

			}
		};


	}
}