#pragma once

#include "./NetworkInitializer.h"

#include <atomic>

#include "scl/IO/Pipeline.h"
#include "scl/Types.h"

namespace scl
{
	class NetworkWorker : public IThreadWorker
	{
	public:
		class IListener
		{
		protected:
			virtual void Accept() = 0;
			virtual void ProcessAccept() = 0;
			static void AddListener(Sp<NetworkWorker> worker, Sp<IListener> listener);

			friend class NetworkWorker;
		};

		NetworkWorker();
		~NetworkWorker();

		NetworkWorker(const NetworkWorker&) = delete;
		NetworkWorker& operator=(const NetworkWorker&) = delete;

		void OnSessionEstablished(Sp<class Session> session);

	private:
		// IThreadWorker
		virtual void Run() sealed override;
		void AddListener(Sp<IListener> listener);

	private:
		std::vector<Sp<class IListener>> _listeners;
		Sp<class NetworkThread> _networkThread;
		static std::atomic<bool> _init;
	};
}