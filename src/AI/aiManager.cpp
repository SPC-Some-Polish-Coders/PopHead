#include "aiManager.hpp" 
#include <cmath>

namespace ph {

AIManager::AIManager()
{
}

std::deque<Direction> AIManager::getZombiePath(const sf::Vector2f zombiePosition) const
{
	if(doesZombieSeePlayer(zombiePosition))
		return getPath(zombiePosition, mPlayerPosition);
	else
		return getRandomPath(zombiePosition);
}

bool AIManager::doesZombieSeePlayer(const sf::Vector2f zombiePosition) const
{
	float legX = std::abs(zombiePosition.x - mPlayerPosition.x);
	float legY = std::abs(zombiePosition.y - mPlayerPosition.y);
	float distanceBetweenZombieAndPlayer = std::hypotf(legX, legY);
	constexpr float maximalDistanceFromWhichZombieSeesPlayer = 200.f;
	return distanceBetweenZombieAndPlayer <= maximalDistanceFromWhichZombieSeesPlayer;
}

std::deque<Direction> AIManager::getPath(const sf::Vector2f startPosition, const sf::Vector2f destination) const
{
	return std::deque<Direction>();
}

std::deque<Direction> AIManager::getRandomPath(const sf::Vector2f startPosition) const
{
	return std::deque<Direction>();
}

}