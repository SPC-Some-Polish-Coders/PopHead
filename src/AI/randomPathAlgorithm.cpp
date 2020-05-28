#include "pch.hpp"
#include "RandomPathAlgorithm.hpp"
#include "Utilities/random.hpp"

namespace ph {

RandomPathAlgorithm::RandomPathAlgorithm(const ObstacleGrid& obstacleGrid, Vec2i startNodePosition)
	:mObstacleGrid(obstacleGrid)
	,mStartNodePosition(startNodePosition)
	,mMaximalWalkableDistance(5)
	,mNumberOfRecurrencyCalls(0)
{
}

Path RandomPathAlgorithm::getRandomPath()
{
	i32 isGonnaWalk = Random::generateNumber(0, 1);
	if(isGonnaWalk)
		return getRandomWalkingPath();
	else
		return getRandomStayingPath();
}

Path RandomPathAlgorithm::getRandomStayingPath()
{
	i32 howMuchStayingInTheSamePlace = Random::generateNumber(1, 3);
	Path path;
	path.resize(howMuchStayingInTheSamePlace);
	std::fill(path.begin(), path.end(), Direction::none);
	return path;
}

Path RandomPathAlgorithm::getRandomWalkingPath()
{
	if(mNumberOfRecurrencyCalls > 5)
		return Path();

	Path path;
	Direction direction = Cast<Direction>(Random::generateNumber(0, 3) * 2);
	u32 walkableDistanceBetweenObstacleNode = getWalkableDistanceBetweenObstacleNodeIn(direction);
	if(walkableDistanceBetweenObstacleNode < 2) {
		++mNumberOfRecurrencyCalls;
		return getRandomPath();
	}
	i32 lengthOfPath = Random::generateNumber(2, walkableDistanceBetweenObstacleNode);
	path.resize(lengthOfPath);
	std::fill(path.begin(), path.end(), direction);
	return path;
}

u32 RandomPathAlgorithm::getWalkableDistanceBetweenObstacleNodeIn(Direction direction)
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
		PH_UNEXPECTED_SITUATION("Direction has to be either east, west, north or south!");
	}
}

u32 RandomPathAlgorithm::getWalkableDistanceBetweenObstacleNodeOnEast()
{
	for(i32 i = 1; i < 7; ++i) {
		if(Cast<i32>(mObstacleGrid.getColumnsCount()) <= mStartNodePosition.x + i)
			return i;
		if(mObstacleGrid.isObstacle(mStartNodePosition.x + i, mStartNodePosition.y))
			return i - 1;
	}
	return mMaximalWalkableDistance;
}

u32 RandomPathAlgorithm::getWalkableDistanceBetweenObstacleNodeOnWest()
{
	for(i32 i = 1; i < 7; ++i) {
		if(mStartNodePosition.x - i < 0)
			return i;
		if(mObstacleGrid.isObstacle(mStartNodePosition.x - i, mStartNodePosition.y))
			return i - 1;
	}
	return mMaximalWalkableDistance;
}

u32 RandomPathAlgorithm::getWalkableDistanceBetweenObstacleNodeToTheNorth()
{
	for(i32 i = 1; i < 7; ++i) {
		if(mStartNodePosition.y - i < 0)
			return i;
		if(mObstacleGrid.isObstacle(mStartNodePosition.x, mStartNodePosition.y - i))
			return i - 1;
	}
	return mMaximalWalkableDistance;
}

u32 RandomPathAlgorithm::getWalkableDistanceBetweenObstacleNodeToTheSouth()
{
	for(i32 i = 1; i < 7; ++i) {
		if(Cast<i32>(mObstacleGrid.getRowsCount()) <= mStartNodePosition.y + i)
			return i;
		if(mObstacleGrid.isObstacle(mStartNodePosition.x, mStartNodePosition.y + i))
			return i - 1;
	}
	return mMaximalWalkableDistance;
}

}
