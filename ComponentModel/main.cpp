#include "Pipeline.h"
#include "Network.h"

using namespace scl;

int main()
{
	auto networkWorker = New<NetworkWorker>();
	auto listener = New<Listener>(networkWorker);
	
	listener->Listen("0.0.0.0", 17777, [](auto d)
	{
		
	});

	Pipeline pipeline;
	pipeline.Push(networkWorker);

	pipeline.Run();
}