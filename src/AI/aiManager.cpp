#include "aiManager.hpp" 

namespace ph {

AIManager::AIManager()
{
}

Direction AIManager::whereShouldIgo(const Zombie*)
{
	return Direction();
}

void AIManager::removeOpponentFromManagedObjects(const Zombie*)
{
}

}