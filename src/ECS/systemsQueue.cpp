#include "systemsQueue.hpp"
#include "Utilities/threadPool.hpp"

namespace ph {
	
	SystemsQueue::SystemsQueue(entt::registry& registry, ThreadPool& threadPool)
		: mRegistry(registry)
		, mThreadPool(threadPool)
	{
	}

	void SystemsQueue::update(float seconds)
	{
		for (auto& system : mSystemsArray)
			system->update(seconds);
	}

	void SystemsQueue::handleEvents(Event e)
	{
		for (auto& system : mSystemsArray)
			system->onEvent(e);
	}

}
