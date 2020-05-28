#pragma once 

#include "obstacleGrid.hpp"
#include "pathData.hpp"

#include <SFML/Graphics.hpp>

namespace ph { 

class RandomPathAlgorithm
{
public:
	RandomPathAlgorithm(const ObstacleGrid&, Vec2i startNodePosition);
	Path getRandomPath();

private:
	Path getRandomStayingPath();
	Path getRandomWalkingPath();
	u32 getWalkableDistanceBetweenObstacleNodeIn(Direction);
	u32 getWalkableDistanceBetweenObstacleNodeOnEast();
	u32 getWalkableDistanceBetweenObstacleNodeOnWest();
	u32 getWalkableDistanceBetweenObstacleNodeToTheNorth();
	u32 getWalkableDistanceBetweenObstacleNodeToTheSouth();

private:
	const ObstacleGrid& mObstacleGrid;
	const Vec2i mStartNodePosition;
	const u32 mMaximalWalkableDistance;
	u32 mNumberOfRecurrencyCalls;
};

} 
