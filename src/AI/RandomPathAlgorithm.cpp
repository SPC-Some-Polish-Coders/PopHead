#include "RandomPathAlgorithm.hpp"
#include "Utilities/random.hpp"
#include "Logs/logs.hpp"
#include <algorithm>

namespace ph {

RandomPathAlgorithm::RandomPathAlgorithm(const ObstacleGrid& obstacleGrid, const sf::Vector2u startNodePosition)
	:mObstacleGrid(obstacleGrid)
	,mStartNodePosition(startNodePosition)
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
	case ph::Direction::east: {
		for(unsigned i = 1; i < 7; ++i) {
			if(mObstacleGrid.size() <= mStartNodePosition.x + i)
				return 10;
			if(mObstacleGrid[mStartNodePosition.x + i][mStartNodePosition.y])
				return i - 1;
		}
		return 10;
	}
	case ph::Direction::west: {
		for(unsigned i = 1; i < 7; ++i) {
			if(mStartNodePosition.x - i < 0)
				return 10;
			if(mObstacleGrid[mStartNodePosition.x - i][mStartNodePosition.y])
				return i - 1;
		}
		return 10;
	}
	case ph::Direction::north: {
		for(unsigned i = 1; i < 7; ++i) {
			if(mStartNodePosition.y - i < 0)
				return 10;
			if(mObstacleGrid[mStartNodePosition.x][mStartNodePosition.y - i])
				return i - 1;
		}
		return 10;
	}
	case ph::Direction::south:
		for(unsigned i = 1; i < 7; ++i) {
			if(mObstacleGrid.size() <= mStartNodePosition.y + i)
				return 10;
			if(mObstacleGrid[mStartNodePosition.x][mStartNodePosition.y + i])
				return i - 1;
		}
		return 10;
	default:
		PH_UNEXPECTED_SITUATION("Direction has to be either east, west, north or south!")
	}
}

}
