#include "aiManager.hpp" 

namespace ph {

AIManager::AIManager()
{
}

std::deque<Direction> AIManager::whereShouldIgo(const Zombie*)
{
	return std::deque<Direction>();
}

void AIManager::removeOpponentFromManagedObjects(const Object*)
{
}

}