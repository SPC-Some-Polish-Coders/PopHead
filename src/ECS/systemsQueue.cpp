#include "systemsQueue.hpp"

namespace ph {

void SystemsQueue::update(float seconds)
{
	for (auto& system : mSystemsArray)
		system->update(seconds);
}

void SystemsQueue::appendSystem(std::unique_ptr<System>&& system)
{
	mSystemsArray.push_back(std::move(system));
}

}
