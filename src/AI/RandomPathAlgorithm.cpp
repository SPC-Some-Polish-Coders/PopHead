#include "RandomPathAlgorithm.hpp"
#include "Utilities/random.hpp"
#include "Logs/logs.hpp"
#include <algorithm>

namespace ph {

RandomPathAlgorithm::RandomPathAlgorithm(const ObstacleGrid& obstacleGrid, const sf::Vector2u startNodePosition)
	:mObstacleGrid(obstacleGrid)
	,mStartNodePosition(startNodePosition)
	,mMaximalWalkableDistance(5)
{
}

Path RandomPathAlgorithm::getRandomPath()
{
	Path path;
	Direction direction = static_cast<Direction>(Random::generateNumber(0, 3));
	const unsigned walkableDistanceBetweenObstacleNode = getWalkableDistanceBetweenObstacleNodeIn(direction);
	if(walkableDistanceBetweenObstacleNode < 2)
		return getRandomPath();
	int lengthOfPath;
	if(walkableDistanceBetweenObstacleNode > 5)
		lengthOfPath = Random::generateNumber(2, 5);
	else
		lengthOfPath = Random::generateNumber(2, walkableDistanceBetweenObstacleNode);
	path.resize(lengthOfPath);
	std::fill(path.begin(), path.end(), direction);
	return path;
}

unsigned RandomPathAlgorithm::getWalkableDistanceBetweenObstacleNodeIn(Direction direction)
{
	switch(direction)
	{
	case ph::Direction::east:
		return getWalkableDistanceBetweenObstacleNodeOnEast();
	case ph::Direction::west:
		return getWalkableDistanceBetweenObstacleNodeOnWest();
	case ph::Direction::north:
		return getWalkableDistanceBetweenObstacleNodeToTheNorth();
	case ph::Direction::south:
		return getWalkableDistanceBetweenObstacleNodeToTheSouth();
	default:
		PH_UNEXPECTED_SITUATION("Direction has to be either east, west, north or south!")
	}
}

unsigned RandomPathAlgorithm::getWalkableDistanceBetweenObstacleNodeOnEast()
{
	for(unsigned i = 1; i < 7; ++i) {
		if(mObstacleGrid.size() <= mStartNodePosition.x + i)
			return i;
		if(mObstacleGrid[mStartNodePosition.x + i][mStartNodePosition.y])
			return i - 1;
	}
	return mMaximalWalkableDistance;
}

unsigned RandomPathAlgorithm::getWalkableDistanceBetweenObstacleNodeOnWest()
{
	for(unsigned i = 1; i < 7; ++i) {
		if(mStartNodePosition.x - i < 0)
			return i;
		if(mObstacleGrid[mStartNodePosition.x - i][mStartNodePosition.y])
			return i - 1;
	}
	return mMaximalWalkableDistance;
}

unsigned RandomPathAlgorithm::getWalkableDistanceBetweenObstacleNodeToTheNorth()
{
	for(unsigned i = 1; i < 7; ++i) {
		if(mStartNodePosition.y - i < 0)
			return i;
		if(mObstacleGrid[mStartNodePosition.x][mStartNodePosition.y - i])
			return i - 1;
	}
	return mMaximalWalkableDistance;
}

unsigned RandomPathAlgorithm::getWalkableDistanceBetweenObstacleNodeToTheSouth()
{
	for(unsigned i = 1; i < 7; ++i) {
		if(mObstacleGrid.size() <= mStartNodePosition.y + i)
			return i;
		if(mObstacleGrid[mStartNodePosition.x][mStartNodePosition.y + i])
			return i - 1;
	}
	return mMaximalWalkableDistance;
}

}
