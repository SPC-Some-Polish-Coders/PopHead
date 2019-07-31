#include "aiManager.hpp" 
#include "aStar.hpp"
#include <algorithm>
#include <cmath>

namespace ph {

Path AIManager::getZombiePath(const sf::Vector2f zombiePosition) const
{
	if(doesZombieSeePlayer(zombiePosition))
		return getPath(zombiePosition, mPlayerPosition);
	else
		return getRandomPath(zombiePosition);
}

void AIManager::setPlayerPosition(const sf::Vector2f playerPosition) 
{ 
	this->mPlayerPosition = playerPosition; 
	mHasPlayerMovedSinceLastUpdate = true; 
}

void AIManager::registerMapSize(const sf::Vector2u mapSizeInTiles)
{
	mGrid.resize(mapSizeInTiles.x);
	for(auto& column : mGrid) {
		column.resize(mapSizeInTiles.y);
		std::fill(column.begin(), column.end(), false);
	}
}

void AIManager::registerObstacle(const sf::Vector2f collisionBodyPosition)
{
	const unsigned gridPositionX = static_cast<unsigned>(collisionBodyPosition.x) / mSpotSideLength;
	const unsigned gridPositionY = static_cast<unsigned>(collisionBodyPosition.y) / mSpotSideLength;
	mGrid[gridPositionX][gridPositionY] = true;
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
	constexpr float maximalDistanceFromWhichZombieSeesPlayer = 600.f;
	return distanceBetweenZombieAndPlayer <= maximalDistanceFromWhichZombieSeesPlayer;
}

Path AIManager::getPath(const sf::Vector2f startPosition, const sf::Vector2f destinationPosition) const
{
	AStar a(mGrid);
	return a.getPath(toNodePosition(startPosition), toNodePosition(destinationPosition));
}

sf::Vector2u AIManager::toNodePosition(sf::Vector2f position) const
{
	// TODO: Support other tile size then 16x16
	return sf::Vector2u(position.x / 16, position.y / 16);
}

Path AIManager::getRandomPath(const sf::Vector2f startPosition) const
{
	return Path();
}

}