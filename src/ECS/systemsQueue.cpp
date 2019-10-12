#include "systemsQueue.hpp"

namespace ph {


void SystemsQueue::appendSystem(std::unique_ptr<System>&& system)
{
	mSystemsArray.push_back(std::move(system));
}

	void SystemsQueue::update(float seconds)
	{
		for (auto& system : mSystemsArray)
			system->update(seconds);
	}
}
