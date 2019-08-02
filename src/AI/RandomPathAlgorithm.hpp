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
	unsigned getWalkableDistanceBetweenObstacleNodeIn(Direction);

private:
	const ObstacleGrid& mObstacleGrid;
	const sf::Vector2u mStartNodePosition;
};

} 
