#include "aiManager.hpp" 
#include <algorithm>
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

void AIManager::registerMapSize(const sf::Vector2u mapSizeInTiles)
{
	mGrid.resize(mapSizeInTiles.y);
	for(auto& row : mGrid) {
		row.resize(mapSizeInTiles.x);
		std::fill(row.begin(), row.end(), AreaType::walkable);
	}
}

void AIManager::registerStaticCollisionBody(const sf::Vector2f collisionBodyPosition)
{
	const int gridPositionX = collisionBodyPosition.x / spotSideLength;
	const int gridPositionY = collisionBodyPosition.y / spotSideLength;
	mGrid[gridPositionX][gridPositionY] = AreaType::obstacle;
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