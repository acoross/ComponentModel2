#pragma once

#pragma comment(lib, "mswsock.lib")
#pragma comment(lib, "ws2_32.lib")

#include <WS2tcpip.h>
#include <atomic>

#include "Pipeline.h"

namespace scl
{
	// network
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

	private:
		// IThreadWorker
		virtual void Run() sealed override;
		void ProcessAccept();

		void AddListener(Sp<IListener> listener);

	private:
		std::vector<Sp<class IListener>> _listeners;
		static std::atomic<bool> _init;
	};
}