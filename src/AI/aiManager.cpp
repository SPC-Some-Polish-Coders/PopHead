#include "pch.hpp"
#include "aiManager.hpp" 
#include "aStarAlgorithm.hpp"
#include "RandomPathAlgorithm.hpp"

namespace ph {

PathMode AIManager::getZombiePath(Vec2 zombiePosition) const
{
	if (!mIsPlayerOnScene || mAIMode == AIMode::zombieAlwaysWalkRandomly)
		return { getRandomPath(zombiePosition) };
	if (mAIMode == AIMode::zombieAlwaysLookForPlayer)
		return { getPath(zombiePosition, mPlayerPosition), true };

	float distanceToPlayer = getDistanceBetweenZombieAndPlayer(zombiePosition);
	constexpr float maximalDistanceFromWhichZombieSeesPlayer = 285.f;
	constexpr float maximalDistanceFromWhichZombieWalksRandom = 350.f;

	if (distanceToPlayer <= maximalDistanceFromWhichZombieSeesPlayer)
		return { getPath(zombiePosition, mPlayerPosition), true };
	else if (distanceToPlayer <= maximalDistanceFromWhichZombieWalksRandom)
		return { getRandomPath(zombiePosition) };
	return { Path() };
}

bool AIManager::shouldZombiePlayAttackAnimation(Vec2 zombiePosition) const
{
	float distanceBetweenZombieAndPlayer = getDistanceBetweenZombieAndPlayer(zombiePosition);
	return distanceBetweenZombieAndPlayer < 25;
}

void AIManager::setPlayerPosition(Vec2 playerPosition) 
{ 
	this->mPlayerPosition = playerPosition; 
	mHasPlayerMovedSinceLastUpdate = true;
}

void AIManager::registerMapSize(Vec2u mapSizeInTiles)
{
	mObstacleGrid = ObstacleGrid(mapSizeInTiles.x, mapSizeInTiles.y);
}

void AIManager::registerObstacle(Vec2 gridPosition)
{
	mObstacleGrid.registerObstacle(Cast<size_t>(gridPosition.x), Cast<size_t>(gridPosition.y));
}

void AIManager::update()
{
	mHasPlayerMovedSinceLastUpdate = false;
}

float AIManager::getDistanceBetweenZombieAndPlayer(Vec2 zombiePosition) const
{
	float legX = std::abs(zombiePosition.x - mPlayerPosition.x);
	float legY = std::abs(zombiePosition.y - mPlayerPosition.y);
	float distance = std::hypotf(legX, legY);
	return distance;
}

Path AIManager::getPath(Vec2 startPosition, Vec2 destinationPosition) const
{
	auto dest = toNodePosition(destinationPosition);
	if (mObstacleGrid.isObstacle(dest.x, dest.y))
		dest += Vec2i(1, 1);

	AStarAlgorithm a(mObstacleGrid, toNodePosition(startPosition), dest);
	auto path = a.getPath();
	//if (path.size() > 3)
	//	path.erase(path.cbegin() + 3, path.cend());
	return path;
}

Vec2i AIManager::toNodePosition(Vec2 position) const
{
	return Cast<Vec2i>(Vec2(position.x / mTileSize.x, position.y / mTileSize.y));
}

Path AIManager::getRandomPath(Vec2 startPosition) const
{
	RandomPathAlgorithm rpa(mObstacleGrid, toNodePosition(startPosition));
	return rpa.getRandomPath();
}

}
