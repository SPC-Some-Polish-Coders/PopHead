#include "aiManager.hpp" 
#include "aStarAlgorithm.hpp"
#include "RandomPathAlgorithm.hpp"
#include <algorithm>
#include <cmath>

namespace ph {

Path AIManager::getZombiePath(const sf::Vector2f zombiePosition) const
{
	if(mIsPlayerOnScene && doesZombieSeePlayer(zombiePosition))
		return getPath(zombiePosition, mPlayerPosition);
	else
		return getRandomPath(zombiePosition);
}

bool AIManager::shouldZombiePlayAttackAnimation(const sf::Vector2f zombiePosition) const
{
	float legX = std::abs(zombiePosition.x - mPlayerPosition.x);
	float legY = std::abs(zombiePosition.y - mPlayerPosition.y);
	float distanceBetweenZombieAndPlayer = std::hypotf(legX, legY);
	return distanceBetweenZombieAndPlayer < 25;
}

void AIManager::setPlayerPosition(const sf::Vector2f playerPosition) 
{ 
	this->mPlayerPosition = playerPosition; 
	mHasPlayerMovedSinceLastUpdate = true; 
}

void AIManager::registerMapSize(const sf::Vector2u mapSizeInTiles)
{
	mObstacleGrid.resize(mapSizeInTiles.x);
	for(auto& column : mObstacleGrid) {
		column.resize(mapSizeInTiles.y);
		std::fill(column.begin(), column.end(), false);
	}
}

void AIManager::registerObstacle(const sf::Vector2f collisionBodyPosition)
{
	const unsigned gridPositionX = static_cast<unsigned>(collisionBodyPosition.x) / mSpotSideLength;
	const unsigned gridPositionY = static_cast<unsigned>(collisionBodyPosition.y) / mSpotSideLength;
	mObstacleGrid[gridPositionX][gridPositionY] = true;
}

void AIManager::update()
{
	mHasPlayerMovedSinceLastUpdate = false;
}

bool AIManager::doesZombieSeePlayer(const sf::Vector2f zombiePosition) const
{
	float legX = std::abs(zombiePosition.x - mPlayerPosition.x);
	float legY = std::abs(zombiePosition.y - mPlayerPosition.y);
	float distanceBetweenZombieAndPlayer = std::hypotf(legX, legY);
	constexpr float maximalDistanceFromWhichZombieSeesPlayer = 285.f;
	return distanceBetweenZombieAndPlayer <= maximalDistanceFromWhichZombieSeesPlayer;
}

Path AIManager::getPath(const sf::Vector2f startPosition, const sf::Vector2f destinationPosition) const
{
	AStarAlgorithm a(mObstacleGrid);
	return a.getPath(toNodePosition(startPosition), toNodePosition(destinationPosition));
}

sf::Vector2u AIManager::toNodePosition(sf::Vector2f position) const
{
	// TODO: Support other tile size then 16x16
	return static_cast<sf::Vector2u>(position) / 16u;
}

Path AIManager::getRandomPath(const sf::Vector2f startPosition) const
{
	RandomPathAlgorithm rpa(mObstacleGrid, toNodePosition(startPosition));
	return rpa.getRandomPath();
}

}