#pragma once 
 
#include "pathData.hpp"
#include <SFML/Graphics.hpp>

namespace ph { 

class RandomPathAlgorithm
{
public:
	RandomPathAlgorithm(const ObstacleGrids&, const sf::Vector2u startNodePosition);
	Path getRandomPath();

private:
	Path getRandomStayingPath();
	Path getRandomWalkingPath();
	unsigned getWalkableDistanceBetweenObstacleNodeIn(const Direction);
	unsigned getWalkableDistanceBetweenObstacleNodeOnEast();
	unsigned getWalkableDistanceBetweenObstacleNodeOnWest();
	unsigned getWalkableDistanceBetweenObstacleNodeToTheNorth();
	unsigned getWalkableDistanceBetweenObstacleNodeToTheSouth();

private:
	const ObstacleGrids& mObstacleGrid;
	const sf::Vector2i mStartNodePosition;
	const unsigned mMaximalWalkableDistance;
	unsigned mNumberOfRecurrencyCalls;
};

} 
