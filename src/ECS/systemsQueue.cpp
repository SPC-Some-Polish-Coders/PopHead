#include "systemsQueue.hpp"

namespace ph {
	
	SystemsQueue::SystemsQueue(entt::registry& registry)
		: mRegistry(registry)
	{
	}

	void SystemsQueue::update(float seconds)
	{
		for (auto& system : mSystemsArray)
			system->update(seconds);
	}
}
