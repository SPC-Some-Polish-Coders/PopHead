#pragma once 
 
#include "pathData.hpp"
#include <SFML/Graphics.hpp>

namespace ph { 

class RandomPathAlgorithm
{
public:
	RandomPathAlgorithm(const ObstacleGrid&, const sf::Vector2u startNodePosition);
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
	const ObstacleGrid& mObstacleGrid;
	const sf::Vector2u mStartNodePosition;
	const unsigned mMaximalWalkableDistance;
	unsigned mNumberOfRecurrencyCalls;
};

} 
