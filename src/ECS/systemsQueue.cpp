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

	void SystemsQueue::handleEvents(const ActionEvent& event)
	{
		for (auto& system : mSystemsArray)
			system->onEvent(event);
	}

}
