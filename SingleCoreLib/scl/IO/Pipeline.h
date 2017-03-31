#pragma once

#include <vector>
#include "scl/memory.h"

namespace scl
{
	// pipeline & interfaces
	class IThreadWorker
	{
	protected:
		virtual void Run() {}

		friend class Pipeline;
	};

	class Pipeline
	{
	public:
		void Push(Sp<IThreadWorker> worker)
		{
			_workers.push_back(worker);
		}

		void Run()
		{
			for (;;)
			{
				for (auto& worker : _workers)
				{
					worker->Run();
				}
			}
		}

		void RunOnce()
		{
			for (auto& worker : _workers)
			{
				worker->Run();
			}
		}

	private:
		std::vector<Sp<IThreadWorker>> _workers;
	};
}