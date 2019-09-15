#include "aiManager.hpp" 
#include "aStarAlgorithm.hpp"
#include "RandomPathAlgorithm.hpp"

#include <algorithm>
#include <cmath>

namespace ph {

Path AIManager::getZombiePath(const sf::Vector2f zombiePosition) const
{
	if (!mIsPlayerOnScene)
		return Path();

	float distanceToPlayer = getDistanceBetweenZombieAndPlayer(zombiePosition);
	constexpr float maximalDistanceFromWhichZombieSeesPlayer = 285.f;
	constexpr float maximalDistanceFromWhichZombieWalksRandom = 350.f;

	if (distanceToPlayer <= maximalDistanceFromWhichZombieSeesPlayer)
		return getPath(zombiePosition, mPlayerPosition);
	else if (distanceToPlayer <= maximalDistanceFromWhichZombieWalksRandom)
		return getRandomPath(zombiePosition);
	return Path();
}

bool AIManager::shouldZombiePlayAttackAnimation(const sf::Vector2f zombiePosition) const
{
	float distanceBetweenZombieAndPlayer = getDistanceBetweenZombieAndPlayer(zombiePosition);
	return distanceBetweenZombieAndPlayer < 25;
}

void AIManager::setPlayerPosition(const sf::Vector2f playerPosition) 
{ 
	this->mPlayerPosition = playerPosition; 
	mHasPlayerMovedSinceLastUpdate = true;
}

void AIManager::registerMapSize(const sf::Vector2u mapSizeInTiles)
{
	mObstacleGrid = ObstacleGrid(mapSizeInTiles.x, mapSizeInTiles.y);
}

void AIManager::registerObstacle(const sf::Vector2f collisionBodyPosition)
{
	// TODO: It should be checked if collision body is on multiple grids (tiles)
	const auto spotSide = static_cast<float>(mSpotSideLength);
	const auto gridPositionX = static_cast<size_t>(collisionBodyPosition.x / mSpotSideLength);
	const auto gridPositionY = static_cast<size_t>(collisionBodyPosition.y / mSpotSideLength);
	mObstacleGrid.registerObstacle(gridPositionX, gridPositionY);
}

void AIManager::update()
{
	mHasPlayerMovedSinceLastUpdate = false;
}

float AIManager::getDistanceBetweenZombieAndPlayer(const sf::Vector2f zombiePosition) const
{
	float legX = std::abs(zombiePosition.x - mPlayerPosition.x);
	float legY = std::abs(zombiePosition.y - mPlayerPosition.y);
	float distance = std::hypotf(legX, legY);
	return distance;
}

Path AIManager::getPath(const sf::Vector2f startPosition, const sf::Vector2f destinationPosition) const
{
	auto dest = toNodePosition(destinationPosition);
	if (mObstacleGrid.isObstacle(dest.x, dest.y))
		dest += sf::Vector2u(1, 1);

	AStarAlgorithm a(mObstacleGrid, toNodePosition(startPosition), dest);
	auto path = a.getPath();
	//if (path.size() > 3)
	//	path.erase(path.cbegin() + 3, path.cend());
	return path;
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