#pragma once

#include <map>
#include "scl/memory.h"
#include "scl/TypeTraits.h"
#include "scl/TypeInfo.h"
#include "scl/exception.h"

namespace GameEngine
{
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
					pair.second(msg);
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
}